import os

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

commandesJuste = [
			 #Success tests
			 "./minicc",
			 "./minicc -h",
			 "./minicc -b",
			 "./minicc hello.c",
			 "./minicc hello.c -o assembleur.s",
			 "./minicc -o assembleur.s hello.c",
			 "./minicc -o assembleur.s -t 0 hello.c -r 6",
			 "./minicc -o assembleur.s -v hello.c",
			 "./minicc -s hello.c"
			 ]

# Failure tests
commandesFausse = [
			 "./minicc -b hello.c", 
			 "./minicc hello.c random.c",
			 "./minicc -s -v hello.c",
			 "./minicc -t -r 4 hello.c",
			 "./minicc hello.c -o assembleur.s random.c",
			 "./minicc -r 2 hello.c",
			 "./minicc -t 6 hello.c",
			 "./minicc -t 0 -r 8 -o assembleur.s"
			 ]

os.system("make realclean")
os.system("make all")

print(bcolors.HEADER + "============== Début des commandes justes ==============" + bcolors.ENDC)

i=0
for i in range(len(commandesJuste)):
	
	print("Executing " + commandesJuste[i])
	a = os.system(commandesJuste[i])

	if(a == 0):
		print(bcolors.OKGREEN + "OK" + bcolors.ENDC)
	else :
		print(bcolors.FAIL + "K0" + bcolors.ENDC)
		exit(1)

print(bcolors.HEADER + "============== Début des commandes fausses ==============" + bcolors.ENDC)

i=0
for i in range(len(commandesFausse)):
	
	print("Executing " + commandesFausse[i])
	a = os.system(commandesFausse[i])

	if(a == 0):
		print(bcolors.OKGREEN + "OK" + bcolors.ENDC)
		exit(1)
	else :
		print(bcolors.FAIL + "K0" + bcolors.ENDC)


print(bcolors.OKGREEN + "TOUT EST OK" + bcolors.ENDC)

