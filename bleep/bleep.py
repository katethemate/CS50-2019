from cs50 import get_string
from sys import argv


def main():
    
    toBeCensored = get_string("What message would you like to censor?").split()
    file = open(argv[1], 'r')
    banned = []
    print(toBeCensored)
    
    for word in file:
        banned.append(word.strip('\n'))

    for i in range(len(toBeCensored)):
        if toBeCensored[i] in banned:
            toBeCensored[i] = '*'*len(toBeCensored[i])
    toBeCensored = " ".join(toBeCensored)
    print(toBeCensored)
    
if __name__ == "__main__":
    main()
