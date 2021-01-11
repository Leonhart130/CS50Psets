# Usage: python3 roster.py house_name
# Return roster of house from students.db
# Students are ordered by last name
# For students with the same last name, they are be ordered by first name.
import sys
import cs50

# Dictionary to store requested output
requested_otp = dict()


def main(argv):

    # Verify usage
    if len(argv) != 2:
        print("Usage: python3 roster.py house_name")
        exit(1)

    # Init
    house_name = argv[1]
    db = cs50.SQL("sqlite:///students.db")

    # Execute user query
    requested_otp = db.execute("""SELECT first, middle, last, birth
                                FROM students
                                WHERE house = ?
                                ORDER BY last, first""", house_name)
    
    # Wrong usage
    if not requested_otp:
        print("No entries for that house")
        exit(2)
    
    # Return requested output
    for row in requested_otp:
        
        # Init for readability
        first = row["first"]
        middle = row["middle"]
        last = row["last"]
        birth = row["birth"]

        # Print requested output, varies if middle name is NULL
        if middle:
            print(f"{first} {middle} {last}, born {birth}")
        else:
            print(f"{first} {last}, born {birth}")

    exit(0)


if __name__ == "__main__":
    main(sys.argv)