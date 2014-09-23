#! /usr/bin/env python3.4
"""
This is a primeFinder v2.0.
"""

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

if __name__ == "__main__":
    print(p_test_range(10, 2, 5, [2,3,5,7,11,13,17,23])
