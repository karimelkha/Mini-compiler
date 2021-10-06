// Un exemple 2 de programme MiniC
int start = 0;
int end = 100;
void main(){
	bool a = false;
	int i, s = start, e = end;
	int sum = 0;
	for (i = s; i < e; i = i + 1){
		sum = sum + i;
	}
	print("sum: ", sum, "\n");
}