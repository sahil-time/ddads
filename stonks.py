import sys
import csv
import datetime
from datetime import date

#Usage:
#	python3 stonks.py m o f c 2500 AMZN.csv

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

#Command Line Arguments Python
#sys.argv[1] - Day1
#sys.argv[2] - open/close
#sys.argv[3] - Day2
#sys.argv[4] - open/close
#sys.argv[5] - Options Value

################################################################################

def map_day_to_integer(arg):
    if arg == 'm':
        return 0
    if arg == 't':
        return 1
    if arg == 'w':
        return 2
    if arg == 'th':
        return 3
    if arg == 'f':
        return 4

def map_time_to_str(arg):
    if arg == 'o':
        return 'Open'
    else:
        return 'Close'

def map_integer_to_weekday(arg):
    if(arg == 0):
        return 'Mon'
    if(arg == 1):
        return 'Tue'
    if(arg == 2):
        return 'Wed'
    if(arg == 3):
        return 'Thu'
    else:
        return 'Fri'

################################################################################

def pretty_print(day1, day2, date, diff):
    if(diff < 0):
        print(map_integer_to_weekday(day1) + " -> " + map_integer_to_weekday(day2) + " " + date + " = " + bcolors.BOLD + bcolors.FAIL + str(diff) + bcolors.ENDC + "\n")
    else:
        print(map_integer_to_weekday(day1) + " -> " + map_integer_to_weekday(day2) + " " + date + " = " + bcolors.OKBLUE + str(diff) + bcolors.ENDC + "\n")

################################################################################

with open(sys.argv[6], newline='') as csvfile:
    reader = csv.DictReader(csvfile)

    day1 = map_day_to_integer(sys.argv[1]);
    day1_time = map_time_to_str(sys.argv[2]);

    day2 = map_day_to_integer(sys.argv[3]);
    day2_time = map_time_to_str(sys.argv[4]);

    options_val = float(sys.argv[5])

    gross = 0

    print("\n\n")

#Calculate
    for row in reader:
        date_time_str = row['Date']
        date_time_obj = datetime.datetime.strptime(date_time_str, '%Y-%m-%d')


        if(date_time_obj.weekday() == day1):
            day1_price = row[day1_time]

        if(date_time_obj.weekday() == day2):
            day2_price = row[day2_time]
            diff = float(day2_price) - float(day1_price)

            if(diff < 0):
                gross = gross - options_val
            else:
                gross = gross + 100*(diff) - options_val

            pretty_print(day1, day2, date_time_str, diff)

    print("\nNET GROSS: " + str(gross) + "\n")
