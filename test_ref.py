import os
import glob 
import os.path 

refFlag = 1	#Passer à 1 pour utilise le compilateur de référence
options = ""

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

directories = ["./Tests/Syntaxe/OK/",
			   "./Tests/Syntaxe/KO/",
			   "./Tests/Verif/OK/",
			   "./Tests/Verif/KO/",
			   "./Tests/Gencode/OK/",
			   "./Tests/Gencode/KO/"
			   ]
os.system("clear")
os.system("make realclean")
os.system("make all")
ref = "_ref "
#int i
i = 0

for i  in range(len(directories)):
	print(bcolors.HEADER + "\n===============    Verification " + directories[i] + "    ===============\n" + bcolors.ENDC)
	src_files = []
	l = glob.glob(directories[i])
	for root, dirs, files in os.walk(directories[i]): 
		for j in files:
			if ".c" in j:
				src_files.append(os.path.join(root, j)) 
	if src_files == []:
		print("Fichier source introuvable dans " + directories[i])
	#On doit maintenant compiler tout les fichiers
	for f in src_files:
		print("Executing " + bcolors.BOLD + "./minicc" + ref + f + options + bcolors.ENDC)

		a = os.system("./minicc" + ref + f + options)
		if(a == 0):
			print(bcolors.OKGREEN + "OK" + bcolors.ENDC)
		else :
			print(bcolors.FAIL + "K0" + bcolors.ENDC)

		print("Executing " + bcolors.BOLD + "./minicc " + f + options + bcolors.ENDC)
		a = os.system("./minicc " + f + options)
		if(a == 0):
			print(bcolors.OKGREEN + "OK" + bcolors.ENDC)
		else :
			print(bcolors.FAIL + "K0" + bcolors.ENDC)

		print("\n\n")



