# Usage: import.py file.csv
# Generates database from csv file
import sys
import csv
import cs50

# Dictionary to load csv data
csv_dict = dict()


def main(argv):
    
    # Verify usage
    if len(argv) != 2:
        print("Usage: python3 import.py file.csv")
        exit(1)

    # Load csv content
    load_csv(argv[1]) 

    # Create database
    create_database("students.db")

    exit(0)


# Verify usage, load content into dictionary
def load_csv(csv_db):

    # Read file content
    with open(csv_db, "r") as csvfile:
        
        # Init
        reader = csv.DictReader(csvfile)
        for row in reader:
           
            # Format data
            name = row["name"]
            del row["name"]

            # Copy row for person with data on dictionary
            csv_dict[name] = row

    return


# Create new database and fill with data
def create_database(name):

    # Create database
    open(name, "w").close()
    db = cs50.SQL("sqlite:///" + name)

    # Create tables
    db.execute("""CREATE TABLE students (
              id INTEGER PRIMARY KEY AUTOINCREMENT,
              first VARCHAR(255),
              middle VARCHAR(255),
              last VARCHAR(255),
              house VARCHAR(10),
              birth INTEGER)""")

    # Read dictionary data to database
    for person, data in csv_dict.items():
        
        # Init
        names = person.split()
        first = names[0]
        last = names[2] if len(names) == 3 else names[1]
        middle = names[1] if len(names) == 3 else None
        house = data["house"]
        birth = data["birth"]

        # Insert
        db.execute("""INSERT INTO students
                   (first, middle, last, house, birth)
                   VALUES(?, ?, ?, ?, ?)""",
                   first, middle, last, house, birth)

    return


if __name__ == "__main__":
    main(sys.argv)