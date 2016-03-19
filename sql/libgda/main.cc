#include <libgda/libgda.h>
#include <sql-parser/gda-sql-parser.h>

GdaConnection *open_connection (void);
void display_table_contents (GdaConnection *cnc);
void create_table (GdaConnection *cnc);
void insert_data (GdaConnection *cnc);
void update_data (GdaConnection *cnc);
void delete_data (GdaConnection *cnc);

void run_sql_non_select (GdaConnection *cnc, const gchar *sql);

int main (int argc, char *argv[])
{
    gda_init ();

    GdaConnection *cnc;

	/* open connections */
	cnc = open_connection ();
	create_table (cnc);

	insert_data (cnc);
	display_table_contents (cnc);

	update_data (cnc);
	display_table_contents (cnc);

	delete_data (cnc);
	display_table_contents (cnc);

    gda_connection_close (cnc);

    return 0;
}

/*
 * Open a connection to the example.db file
 */
GdaConnection * open_connection ()
{
    GdaConnection *cnc;
    GError *error = NULL;
	GdaSqlParser *parser;

	/* open connection */
    cnc = gda_connection_open_from_string ("SQLite", "DB_DIR=.;DB_NAME=example", NULL,
				       GDA_CONNECTION_OPTIONS_NONE,
				       &error);
    if (!cnc)
    {
        g_print ("Could not open connection to SQLite database in example_db.db file: %s\n",
                 error && error->message ? error->message : "No detail");
        exit (1);
    }

	/* create an SQL parser */
	parser = gda_connection_create_parser (cnc);
	if (!parser) /* @cnc does not provide its own parser => use default one */
		parser = gda_sql_parser_new ();
	/* attach the parser object to the connection */
	g_object_set_data_full (G_OBJECT (cnc), "parser", parser, g_object_unref);

    return cnc;
}

/*
 * Create a "tbl_person" table
 */
void create_table (GdaConnection *cnc)
{
	run_sql_non_select (cnc, "DROP table IF EXISTS tbl_person");
    run_sql_non_select (cnc, "CREATE table tbl_person (person_id string not null primary key, "
                            "name string not null, birthdate date, nation string)");
}

/*
 * Insert some data
 *
 * Even though it is possible to use SQL text which includes the values to insert into the
 * table, it's better to use variables (place holders), or as is done here, convenience functions
 * to avoid SQL injection problems.
 */
void insert_data (GdaConnection *cnc)
{
	typedef struct
    {
		gchar *person_id;
		gchar *name;
		gchar *birthdate;
        gchar *nation;
	} RowData;
	RowData data [] = {
		{"1", "Danny Chillone", "2014-01-01","Austria"},
		{"2", "Tripper Dan", "2014-01-02","England"},
		{"3", "Lang 'D'", "2014-01-03","Germany"},
		{"4", "Harry", "2014-01-04","Italy"},
		{"5", "Potter", "2014-01-05","Askaban"},
	};
	gint i;

	gboolean res;
	GError *error = NULL;
	GValue *v1, *v2, *v3, *v4;

	for (i = 0; i < sizeof (data) / sizeof (RowData); i++)
    {
		v1 = gda_value_new_from_string (data[i].person_id, G_TYPE_STRING);
		v2 = gda_value_new_from_string (data[i].name, G_TYPE_STRING);
        v3 = gda_value_new_from_string (data[i].birthdate, G_TYPE_STRING);
        v4 = gda_value_new_from_string (data[i].nation, G_TYPE_STRING);

		res = gda_connection_insert_row_into_table (cnc, "tbl_person", &error, "person_id", v1, "name", v2, "birthdate", v3, "nation", v4, NULL);

		if (!res)
        {
			g_error ("Could not INSERT data into the 'tbl_person' table: %s\n",
				 error && error->message ? error->message : "No detail");
		}
		gda_value_free (v1);
		gda_value_free (v2);
		gda_value_free (v3);
        gda_value_free (v4);
	}
}

/*
 * Update some data
 */
void update_data (GdaConnection *cnc)
{
	gboolean res;
	GError *error = NULL;
	GValue *v1, *v2, *v3;

	/* update data where person_id is 'p1000' */
	v1 = gda_value_new_from_string ("4", G_TYPE_STRING);
	v2 = gda_value_new_from_string ("Henry", G_TYPE_STRING);

	res = gda_connection_update_row_in_table (cnc, "tbl_person", "person_id", v1, &error, "name", v2, NULL);

	if (!res)
    {
		g_error ("Could not UPDATE data in the 'tbl_person' table: %s\n",
			 error && error->message ? error->message : "No detail");
	}
	gda_value_free (v1);
	gda_value_free (v2);
}

/*
 * Delete some data
 */
void delete_data (GdaConnection *cnc)
{
	gboolean res;
	GError *error = NULL;
	GValue *v;

	/* delete data where name is 'Potter' */
	v = gda_value_new_from_string ("Potter", G_TYPE_STRING);
	res = gda_connection_delete_row_from_table (cnc, "tbl_person", "name", v, &error);
	if (!res)
    {
		g_error ("Could not DELETE data from the 'tbl_person' table: %s\n",
			 error && error->message ? error->message : "No detail");
	}
	gda_value_free (v);

}

/*
 * display the contents of the 'tbl_person' table
 */
void display_table_contents (GdaConnection *cnc)
{
	GdaDataModel *data_model;
	GdaSqlParser *parser;
	GdaStatement *stmt;
	const char *sql = "SELECT person_id, name, birthdate, nation FROM tbl_person";
	GError *error = NULL;

	//parser = g_object_get_data (G_OBJECT (cnc)); // "parser"
    parser = gda_connection_create_parser (cnc);
	stmt = gda_sql_parser_parse_string (parser, sql, NULL, NULL);
	data_model = gda_connection_statement_execute_select (cnc, stmt, NULL, &error);
	g_object_unref (stmt);
    if (!data_model)
        g_error ("Could not get the contents of the 'tbl_person' table: %s\n",
                 error && error->message ? error->message : "No detail");
	gda_data_model_dump (data_model, stdout);
	g_object_unref (data_model);
}

/*
 * run a non SELECT command and stops if an error occurs
 */
void run_sql_non_select (GdaConnection *cnc, const gchar *sql)
{
    GdaStatement *stmt;
    GError *error = NULL;
    gint nrows;
	const gchar *remain;
	GdaSqlParser *parser;

	//parser = g_object_get_data (G_OBJECT (cnc), (gpointer)"parser");
    parser = gda_connection_create_parser (cnc);
	stmt = gda_sql_parser_parse_string (parser, sql, &remain, &error);
	if (remain)
		g_print ("REMAINS: %s\n", remain);

    nrows = gda_connection_statement_execute_non_select (cnc, stmt, NULL, NULL, &error);
    if (nrows == -1)
        g_error ("NON SELECT error: %s\n", error && error->message ? error->message : "no detail");
	g_object_unref (stmt);
}