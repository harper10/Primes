#! /usr/bin/env python3.4
"""
This is a primeFinder v2.0.
"""
from math import sqrt



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
    """  #TODO I need to make sure to add this function on the things that call it
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
    test_nums = [num for num in range(2, end_num+1) if num > plist[-1]]
    new_primes = [num for num in test_nums if p_test(num, plist)]
    print(p_test(24, plist))
    plist += new_primes
    return plist


if __name__ == "__main__":
    #print(23**2)
    #print(sqrt(6203))
    print(p_test(24, [2, 3, 5, 7, 11, 13, 17, 23]))
    print(populate_plist([2, 3, 5, 7, 11, 13, 17, 23], 6203))