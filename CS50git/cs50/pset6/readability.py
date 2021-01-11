# Usage: python readability.py
# Reproduces readability
# Prompt user for a text
# Print required grade to read text based on Coleman-Liau Index
import sys
import string


def main():

    # Prompt user for text to evaluate
    user_text = input("Text: ")

    # Retrieve datas for Coleman-Liau Index
    values = retrieve_data(user_text)

    # Retrieve Coleman-Liau Index from data
    grade = coleman_liau_index(values)

    # Return Index
    if grade >= 16:
        print("Grade 16+")
    elif grade <= 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")

    exit(0)


# Input: sentence Return: num of letters, words, and sentences
def retrieve_data(text):

    # Init    
    PUNCT = ['.', '!', '?']
    text_values = {
        "letters": 0,
        "words": 0,
        "sentences": 0
    }

    # Count letters, sentences, words
    for char in text:
        if char.isalpha():
            text_values["letters"] += 1
        elif char in PUNCT:
            text_values["sentences"] += 1
    text_values["words"] += len(text.split())

    return text_values


# Apply, return Coleman-Liau Index from text data
def coleman_liau_index(text_values):

    # Retrieve data for easier call
    letters = text_values["letters"]
    words = text_values["words"]
    sentences = text_values["sentences"]

    # Calculate l and s variables
    l = float(letters) / float(words) * 100
    s = float(sentences) / float(words) * 100

    # Return index
    return round(0.0588 * l - 0.296 * s - 15.8)


if __name__ == "__main__":
    main()