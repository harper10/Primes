#! /usr/bin/env python3.4
"""
This is a primeFinder v2.0.
"""
from math import sqrt
from datetime import datetime


def p_test_range(num, start, end, plist):
    """
    This file takes in an integer and tests it against all of the primes in 
    plist. If all of the numbers in the list do not divide into num, then
    the function returns a true.  If any of them are found to divide into num, 
    then the function returns false.
    return: boolean
    """
    for p in plist[start:end+1]:
        if num % p == 0:
            return False
    return True


def p_test(num, plist):
    """
    This function takes in a number and tests whether it is a prime number or not.
    It also takes in either a list of prime numbers or a file that contains prime
    numbers. I have not decided yet.
    It will return True if it is Prime, and False otherwise.
    I also am planning on using multiple threads to test prime.
    :param num: int
    :param plist: [int]
    :return: boolean
    """
    """
    if plist[-1]**2 < num:
        print("The list is not populated with enough numbers to test number effectively")
        return None
        #TODO I need to make sure that this is the correct thing that I want to return
    """  #TODO I need to make sure to add this function on the things that call it instead
    interval_list = [p for p in plist if p <= sqrt(num)]
    return p_test_range(num, 0, len(interval_list), interval_list)  #TODO I need to find out how to create threads and replace this call


def populate_plist(plist, end_num):
    """
    This function extends the length of plist by testing all of the numbers after the
    last prime number to the given number
    :param plist: the current prime list
    :param end_num: the last number to test
    :return: [int]
    """
    new_primes = []
    if len(plist) == 0:
        test_nums = [num for num in range(2, end_num+1)]
    else:
        test_nums = [num for num in range(2, end_num+1) if num > plist[-1]]
    for num in test_nums:
        if p_test(num, plist):
            plist += [num]
    #new_primes = [num for num in test_nums if p_test(num, plist + new_primes)]
    return plist + new_primes


def read_plist(filename):
    """
    This file will take in a filename and read the primes off of it
    :param filename:
    :return: [int]
    """
    with open(filename, 'r') as fin:
        plist = fin.readlines()
        plist = [int(p) for p in plist]
    return plist


def save_plist(filename, plist):
    """
    This function takes in a
    :param filename:
    :param plist:
    :return:
    """
    with open(filename, 'w')as fout:
        plist = [str(p)+"\n" for p in plist]
        fout.writelines(plist)
    return

if __name__ == "__main__":
    #print(23**2)
    #print(sqrt(6203))
    #print(p_test(2, [2, 3, 5, 7, 11, 13, 17, 23]))
    #print(populate_plist([], 6203))
    #print(populate_plist(read_plist('./PrimeList'), 6203))
    n = 123456789
    a = datetime.now()
    plist = read_plist('./plist2.txt')
    plist = populate_plist(plist, 60000)
    b = datetime.now()
    print("The prime list took {} to read/populate up to 60000 primes".format(b - a))
    print(p_test(n, plist))
    c = datetime.now()
    print("Testing {} took {} time".format(n, (c-b)))
    save_plist('./plist2.txt', plist)
    #The prime list took 0:00:46.870586 to populate up to 50000
    #The prime list took 0:00:00.003392 to read up to 50000 primes