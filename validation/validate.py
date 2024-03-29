import os

b3Log = "/media/anoni/Dados/Documentos/USP/FASTDecoder/FASTEngineMD/validation/fixtestlogb3-3.txt" #"fixtestlogb3-3.txt"
myLog =  "/media/anoni/Dados/Documentos/USP/FASTDecoder/FASTEngineMD/validation/logFIX51-FASTEngineMD.txt" #"logFIX51-FASTEngineMD.txt"

fixlogb3 = open(b3Log, "r") #open the file
fixlogmine = open(myLog, "r")

num_msgsB3 = sum(1 for line in fixlogb3) #number of messages in the log
num_msgsMine = sum(1 for line in fixlogmine)

fixlogb3 = open(b3Log, "r")
fixlogmine = open(myLog, "r")

num_msgs = num_msgsB3 if num_msgsB3 > num_msgsMine else num_msgsMine #considerate the bigger file log
noErrors = 0
noHits = 0

for x in range(0, num_msgs - 1):
    
    list1 = fixlogb3.readline() #read the next line
    list2 = fixlogmine.readline()

    line1 = list1.split("|") #split the fields' values
    line2 = list2.split("|")

    isEqual = 1
    findField = 0

    for i in line1: #logB3 ⊂ myLog
        for j in line2:
            if j == i:
                findField = 1
        if findField == 0:
            isEqual = 0
        findField = 0

    for i in line2: #myLog ⊂ B3
        for j in line1:
            if j == i:
                findField = 1
        if findField == 0:
            isEqual = 0
        findField = 0

    if(isEqual == 0): #false
        noErrors += 1
        print("\nB3:   ", list1)
        print("Mine: ", list2)
        print("-------------------------------------------------------------------------")


print("\n-----------------------------------------\n")
print("Report: ")
print("Number of messages:", num_msgs)
print("Number of messages with erros:", noErrors)
print("{:.2f}".format(noErrors/num_msgs * 100), "% of errors \n")

fixlogb3.close() #close the file
fixlogmine.close()
os.remove(myLog) #delete file