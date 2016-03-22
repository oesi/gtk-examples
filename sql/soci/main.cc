#include "soci.h"
#include "sqlite3/soci-sqlite3.h"
#include <iostream>
#include <exception>
#include <string>

using namespace soci;

class person
{
public:
    void open_connection ();
    void display_table_contents ();
    void create_table ();
    void insert_data ();
    void update_data ();
    void delete_data ();

private:
    session sql;
};

int main (int argc, char *argv[])
{
    person p;

    p.open_connection();
	p.create_table ();

	p.insert_data ();
	p.display_table_contents ();

    std::cout << std::endl;

	p.update_data ();
	p.display_table_contents ();

    std::cout << std::endl;

	p.delete_data ();
	p.display_table_contents ();
    
    return 0;
}

void person::open_connection ()
{
    sql.open(sqlite3, "example.db");
}
/*
 * Create a table
 */
void person::create_table ()
{
    try
    {
        sql << "DROP TABLE IF EXISTS tbl_person;";
        sql << "CREATE TABLE tbl_person(person_id integer NOT NULL primary key, name string not null, birthdate string, nation string);";
    }
    catch (std::exception const &e)
    {
        std::cout << "Error setting Up Database" << e.what() << std::endl;
    }
}

/*
 * Insert some data
 */
void person::insert_data ()
{
    try
    {
        sql << "INSERT INTO tbl_person(person_id, name, birthdate, nation) VALUES('1', 'Danny Chillone', '2014-01-01','Austria');";
        sql << "INSERT INTO tbl_person(person_id, name, birthdate, nation) VALUES('2', 'Tripper Dan', '2014-01-02','England');";
        sql << "INSERT INTO tbl_person(person_id, name, birthdate, nation) VALUES('3', 'Lang D', '2014-01-03','Germany');";
        sql << "INSERT INTO tbl_person(person_id, name, birthdate, nation) VALUES('4', 'Harry', '2014-01-04','Italy');";
        sql << "INSERT INTO tbl_person(person_id, name, birthdate, nation) VALUES('5', 'Potter', '2014-01-05','Askaban');";
    }
    catch (std::exception const &e)
    {
        std::cout << "Error Inserting Data:" << e.what() << std::endl;
    }
}

/*
 * Update some data
 */
void person::update_data ()
{
    try
    {
        sql << "UPDATE tbl_person SET name='Henry' WHERE person_id=4;";
    }
    catch (std::exception const &e)
    {
        std::cout << "Error Inserting Data:" << e.what() << std::endl;
    }
}

/*
 * Delete some data
 */
void person::delete_data ()
{
    try
    {
        sql << "DELETE FROM tbl_person WHERE name='Potter';";
    }
    catch (std::exception const &e)
    {
        std::cout << "Error Inserting Data:" << e.what() << std::endl;
    }
}

/*
 * display the contents of the 'products' table
 */
void person::display_table_contents ()
{
    rowset<row> rs = (sql.prepare << "SELECT person_id, name, birthdate, nation FROM tbl_person");

    // iteration through the resultset:
    for (rowset<row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
    {
        row const& row = *it;

        // dynamic data extraction from each row:
        std::cout << "Id: " << row.get<int>(0);
        std::cout << " Name: " << row.get<std::string>(1);
        std::cout << " birthdate: " << row.get<std::string>(2);
        std::cout << " nation: " << row.get<std::string>(3) << std::endl;
    }
}