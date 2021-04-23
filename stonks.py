import sys
import os
import csv
import heapq
import datetime
from datetime import date

#Usage:
#	python3 stonks.py m o f c 2500 AMZN.csv
#	Download AMZN.csv etc. from https://finance.yahoo.com/quote/AMZN/history?period1=1609632000&period2=1619049600&interval=1d&filter=history&frequency=1d&includeAdjustedClose=true

GLOBAL_LIST = []

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

    file1 = open("STONKS_DATASET.h", "a")
#Playing safe and trying to maximize loss
    if(diff < 0):
        diff = diff - 1
    file1.write(str(int(diff)) + ",\n")
    file1.close()

################################################################################

with open(sys.argv[6], newline='') as csvfile:
    print("\n")

    try:
        os.remove(os.getcwd() + "/STONKS_DATASET.h")
    except:
        print("...")

    reader = csv.DictReader(csvfile)

    day1 = map_day_to_integer(sys.argv[1]);
    day1_time = map_time_to_str(sys.argv[2]);

    day2 = map_day_to_integer(sys.argv[3]);
    day2_time = map_time_to_str(sys.argv[4]);

    options_val = float(sys.argv[5])

    gross = 0

    file1 = open("STONKS_DATASET.h", "a")
    file1.write("#ifndef __STONKS_DATASET_H__\n")
    file1.write("#define __STONKS_DATASET_H__\n")
    file1.write("\nconst int STONKS_DATA[] = {\n")
    file1.close()

#Calculate
    for row in reader:
        date_time_str = row['Date']
        date_time_obj = datetime.datetime.strptime(date_time_str, '%Y-%m-%d')


        if(date_time_obj.weekday() == day1):
            day1_price = row[day1_time]

        if(date_time_obj.weekday() == day2):
            day2_price = row[day2_time]
            diff = float(day2_price) - float(day1_price)
            GLOBAL_LIST.append(diff)	#Do data analysis on this list

            if(diff < 0):
                gross = gross - options_val
            else:
                gross = gross + 100*(diff) - options_val

            pretty_print(day1, day2, date_time_str, diff)

    #Footers in the file
    file1 = open("STONKS_DATASET.h", "a")
    file1.write("};")
    file1.write("\n\n#endif")
    file1.close()

    print("\nNET GROSS: " + str(gross) + "\n")

################################################################################

#DATA ANALYSIS ON THE LIST

################################################################################

total_pos = 0
total_neg = 0
max_contig_pos = 0
max_contig_neg = 0

if GLOBAL_LIST[0] < 0:
    contig_pos = 0
else:
    contig_pos = 1

tmp = 0

for val in GLOBAL_LIST:
    if val < 0:
        #Logic to calculate max contiguous surges and plummets
        if(contig_pos == 1):
            if(max_contig_pos < tmp):
                max_contig_pos = tmp
            contig_pos = 0
            tmp = 0

        total_neg += 1
        tmp  += 1
    else:
        #Logic to calculate max contiguous surges and plummets
        if(contig_pos == 0):
            if(max_contig_neg < tmp):
                max_contig_neg = tmp
            contig_pos = 1
            tmp = 0

        total_pos += 1
        tmp += 1

print("Total Surges: " + str(total_pos) + "\nTotal Plummets: " + str(total_neg) + "\nMax Continuous Surges: " + str(max_contig_pos) + "\nMax Continuous Plummets: " + str(max_contig_neg))
print("\n")

################# HEAP EXPERIMENT #################
li = [9, 5, 4, 3, 2, 1]
heapq.heapify(li)
print(li)
heapq.heappush(li, 0)
print(li)
heapq.heapreplace(li, -1)
print(heapq.nlargest(3,li))
print(li)
