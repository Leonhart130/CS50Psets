# Usage: python dna.py data.csv sequence.txt
# Read and store database to dictionary
# Read sequence of user DNA and determine STR's origins from dictionary
import sys
import csv

# Init global database dictionary
data_dict = dict()


def main(argv):

    # Verify usage
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    # Store path variables
    data_path = argv[1]
    sequence_path = argv[2]

    # Load dictionary data to dictionary
    load_data(data_path)

    # Load sequence file
    with open(sequence_path, 'r') as file:
        sequence = file.read().replace('\n', '')

    # Retrieve frequencies of STR from user sequence
    user_data = get_user_data(sequence)

    # Return similarity result with database
    for person, data in data_dict.items():
        if data == user_data:
            print(person)
            exit(0)
    
    print("No match")

    exit(0)


# Load csv file to data_dict
def load_data(csv_data):

    # Open file
    with open(csv_data, 'r') as file:

        # Init
        reader = csv.DictReader(file)
        for row in reader:

            # Format data
            name = row["name"]
            del row["name"]

            # Copy data to dictionary
            data_dict[name] = row

    return


# Return user STR data as dictionary
def get_user_data(user_sequence):

    # Init user dictionary
    user_freq = dict()
    
    for person, data in data_dict.items():
        for sequence, frequency in data.items():
            user_freq[sequence] = '0'
        break
    
    # Retrieve frequences of sequences
    for sequence, frequence in user_freq.items():
        user_freq[sequence] = str(get_freq(sequence, user_sequence))

    return user_freq


# Retrieve STR for a sequence
def get_freq(db_seq, user_seq):

    # Init
    NEXT_SEQ = len(db_seq)
    max_STR = 0
    current_STR = 0 
   
    # Do while loop counting STR
    for i in range(len(user_seq) - NEXT_SEQ):
       
        # Init sequence
        sequence = user_seq[i : i + NEXT_SEQ]

        while True:
           
            if sequence != db_seq:
                max_STR = current_STR if current_STR > max_STR else max_STR
                current_STR = 0
                break
            
            current_STR += 1
            i += NEXT_SEQ
            sequence = user_seq[i : i + NEXT_SEQ]
                
    
    return max_STR


if __name__ == "__main__":
    main(sys.argv)
