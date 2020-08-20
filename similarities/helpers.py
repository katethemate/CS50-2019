from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""
    lines = []
    a = a.split('\n')
    b = b.split('\n')

    for line in a:
        if line in b and line not in lines:
            lines.append(line)

    return lines


def sentences(a, b):
    """Return sentences in both a and b"""

    sentences = []
    a = sent_tokenize(a)
    b = sent_tokenize(b)

    for sent in a:
        if sent in b and sent not in sentences:
            sentences.append(sent)

    return sentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    substrings = []

    a_substrings = []
    b_substrings = []

    for x in range(len(a)):
        a_substrings.append(a[x:x+n])
        b_substrings.append(b[x:x+n])

    for substring in a_substrings:
        if substring in b_substrings and substring not in substrings:
            substrings.append(substring)

    return substrings
