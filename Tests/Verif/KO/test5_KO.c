// void a;  variable of type void.
int start = 0;
int end = 100;
void main(){
	void a;
	int i , s = start, e = end;
	int sum = 0;
	for (i = s; i < e; i = i + 1){
		sum = sum + i;
	}
	print("sum: ", sum, "\n");
}