//implement libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*
*define a struct node, typedef it to node
*/
typedef struct node {
	char songName [25];
	int duration,pointer; //necessary data fields and pointers
	struct node *next;
	struct node *chrono_next,*duration_next,*alpha_next,*random_next;
}node;
//static data fields to use printList method
static int chrono = 1;
static int durate = 2;  
static int alpha = 3;
static int randomy = 4;
/*
*definition of the function used in proejct
*/
struct node * insertNode(struct node * root,char *songName, int duration);
struct node * insertHeadNodes(struct node * root);
struct node * deleteNode(struct node * root, char *songName);
struct node * deleteWithIndex(struct node * root,int index);
void printList(struct node * root);
void nameBufferCleaner(char *nameBuffer);
void nameBufferUpdater(char *nameBuffer);
struct node * sortChronoOrder(struct node * root);
struct node * sortDuration_TimeOrder(struct node *root);
struct node * sortAlphaOrder(struct node * root);
struct node * sortRandomOrder(struct node * root);
void swap(struct node * ptr1,struct node * ptr2);
void updateAllAndPrint(struct node * chrono_head,struct node * duration_head,struct node * alpha_head, struct node * random_head);
void swapInts(int *a,int *b);
void randomize ( int arr[], int n );
void writerFunc(struct node * root, FILE *outFile);
int length(struct node * root);

/* main function */
int main(){
	//file pointer to open the file
	FILE *fp = fopen("songs.txt","r");
	if(fp == NULL){
		//if file does not exit, print a message
		printf("file is not found..\n");
	} 
	//read inputs from the file char by char with ch variable
	char ch = fgetc(fp);
	char nameBuffer [25] = {'\0'}; //(char*)malloc(sizeof(char)*25); //for song same
	int count = 0,duration,minute,second; // song's other properties

	/*definitions of the nodes as NULL first*/
	struct node * chrono_head = NULL; //(node*)malloc(sizeof(node)); chrono_head->pointer = chrono;
	struct node * duration_head = NULL; //(node*)malloc(sizeof(node)); duration_head->pointer = durate;
	struct node * alpha_head = NULL; //(node*)malloc(sizeof(node));	alpha_head -> pointer = alpha;
	struct node * random_head = NULL; // (node*)malloc(sizeof(node)); random_head->pointer = randomy;
	//line counter to avoid en of the file errors
	int lineCounter = 0;
	//while loop with if fp is not at the end of the file condition to read inputs
	while(!feof(fp)){
		//reading minute and second properties with ascii values
		if(ch >= '0' && ch <= ':'){
			fscanf(fp,"%d:%d",&minute,&second);
			//set the duration of the song
			duration = minute*60 + second;
		}
		else if(ch == '\t')	{
			//song name is taken completely assign -1 to count
			//cause namebuffer has been creating with count value
			count = -1;
		}
		//end of the lines with control EOF
		else if(ch == '\n' && ++lineCounter != 9){
			//delete the spaces in the nameBuffer
			nameBufferUpdater(nameBuffer);
			//insert the songName with pure shape and duration properties to list
			chrono_head = insertNode(chrono_head,nameBuffer,duration);
			//convert to previous songname letters to -> ' ' 
			nameBufferCleaner(nameBuffer); 
		}
		else {
			//now create the songName character by character with count
			nameBuffer[count++] = ch;
		}
		//pass to the next character
		ch = fgetc(fp);
	}
	//menu message
	char * menu = "\nEnter your choise:\n1 to insert a song into the list.\n2 to delete a song from the list.\n3 to print the songs in the list.\n4 to print the songs to an output file.\n5 to end.\n";
	//update and print method for nodes
	updateAllAndPrint(chrono_head,duration_head,alpha_head,random_head);
	//true value = 1 for infinite loop while true is 1 
	int true = 1;
	while(true){
		//print the menu
		printf("%s",menu);
		//define the choise as integer
		int choise; char choi; char chois[25]; 
		//if choise is integer
		if(scanf("%d:",&choise) == 1){
			//but not a valid, print message 
			if(choise <= 0 || choise > 5){
				printf("please enter a valid integer between 1 and 5\n");
			}
		}
		//if literally invalid input as string, print the message and return back
		else if(scanf("%s",chois)) printf("please enter an integer..\n");
		//situations according to choise's value
		switch(choise){
		//case 1 --> inserting a node to the list
		case 1: {
			//define a songName and minute, second for duration property of the node
			char songName[50] = {'\0'};
			int i = 0,minute,second,duration,ch;
			printf("Enter a song name with duration: \n");
			//take to the input with getchar for taking together string name and integer minute and second
			ch = getchar();
			while((ch = getchar()) != '\n'){
				//if ch is tab next ch's is minute and second
				if(ch == '\t'){
				//put the values minute and second
				scanf("%d:%d",&minute,&second);
				}else{
				//the part until the tab ch is songName
				songName[i++] = ch;
        		}
			}
			//calculate duration and insert to the node with songName
			duration = minute*60+second;
			chrono_head = insertNode(chrono_head,songName,duration);
			//end of the case
			break;
		 }
		 //case 2--> delete a node from the list
		 case 2:  {
			 //define song properties to take as input
			 char songName[25] = {'\0'};
			int i = 0,minute,second,duration,ch;
			printf("Enter a song name: \n");
			ch = getchar();
			//like up there, take the songName
			while((ch = getchar()) != '\n'){
				if(ch == '\t'){
				scanf("%d:%d",&minute,&second);
				}else{
				songName[i++] = ch;
        		}
			}
			//delete the songName from the list with deleteNode method
			chrono_head = deleteNode(chrono_head,songName);
			//end of the case
			 break;
		 }
		 //case 3-->print the list
		 case 3: {
			 //simply call the method that updates the methods and prints them
			 updateAllAndPrint(chrono_head,duration_head,alpha_head,random_head);
			 //end of the case
			 break;
		 }
		 //case 4--> print the songs to a file
		 case 4: {
			 	//define a fileName and take as input it from the user
			 	char fileName[50];
				printf("Enter a file name:\n");
				//use scanner cause a only one string
				scanf("%s",fileName);
				//define a file pointer and open a file with write method to write on the songs in it
			 	FILE * outFile = fopen(fileName,"w");
				 /*
				 *update the nodes and call writerFunc method to write on the file
				 */
				chrono_head = sortChronoOrder(chrono_head);
				writerFunc(chrono_head,outFile);
				alpha_head = sortAlphaOrder(chrono_head);
				writerFunc(alpha_head,outFile);
				duration_head = sortDuration_TimeOrder(chrono_head);
				writerFunc(duration_head,outFile);
				random_head = sortRandomOrder(chrono_head);
				writerFunc(random_head,outFile); 			
				//information message
				printf("Output is printed to the file %s",fileName);	
				//end of the case 
				break;
		 }
		 //case 5--> end of the program 
		 case 5: {
			 printf("Program is end..\n");
			 //true value is 0 and finish the loop
			 true = 0;
			}
	 	}
	}
	/*duration_head = chrono_head->chrono_next; 
	duration_head->chrono_next = alpha_head;
	alpha_head->chrono_next = random_head;
	random_head->chrono_next = NULL;

	duration_head->duration_next = random_head;
	random_head->duration_next = chrono_head;
	chrono_head->duration_next = alpha_head;
	alpha_head->duration_next = NULL;

	alpha_head->alpha_next = duration_head;
	duration_head->alpha_next = chrono_head;
	chrono_head->alpha_next = random_head;
	random_head->alpha_next = NULL;

	random_head->random_next = alpha_head;
	alpha_head->random_next = duration_head;
	duration_head->random_next = chrono_head;
	chrono_head->random_next = NULL;
*/
}
/*
*insertNode to insert a node with songName and duration properties
*/
struct node * insertNode(struct node * root,char *songName, int duration){
	//define an iterator that is equal to root and move it in the list
	struct node * iter = root;
		if(iter == NULL){
			//if first node doesn't exit, define it 
			iter = (struct node*)malloc(sizeof(struct node));
			//copy the given songName to node's songName and give duration
			strcpy(iter->songName,songName);
			iter->duration = duration;
			//assign the next node as null to insert new nodes
			iter->next = NULL;
			return iter;
		}
		//find the node that's next is null
    	while(iter-> next != NULL){
        	iter = iter-> next;
    	}
			//and create a new node over there with given properties songName and duration
			iter -> next = (node*)malloc(sizeof(struct node));
			strcpy(iter->next->songName,songName);
			iter->next->duration = duration;
			//and assign the null again
			iter->next->next = NULL;
	//return the head pointer
	return root;
} 
/*
*connecting to head pointers to each other likewise insertNode method
*/
struct node * insertHeadNodes(struct node * root){
	struct node * iter = root;
	if(iter == NULL){
		iter = (struct node *)malloc(sizeof(struct node));
		iter->chrono_next = NULL;
		return iter;
	}
	while(iter-> chrono_next != NULL){
        	iter = iter-> chrono_next;
    	}
			iter -> chrono_next = (node*)malloc(sizeof(struct node));
			iter->chrono_next->chrono_next = NULL;
	return root;
}
//deleteNode method to delete a node from the list based on the songName
struct node * deleteNode(struct node * root, char *songName){
	//define two iterator pointer and temp pointer
	struct node * iter = root ,*iter2 = root, *temp;
	//if head pointer's songName matched the given parameter
    if(strcmp(root->songName,songName) == 0){
		//printf("in the strcmp..\n");
        temp = iter2; // assign temp to node that will be deleted
		temp->next = iter2->next; //to do not lose the head pointer's next assign to next one
        iter2 = iter2-> next; //jump to the next node
		printf("The song '%s' is deleted from the list..\n\n",temp->songName);
		//free the head pointer
        free(temp);
		//return the new head pointer
        return iter2;
}
	//return while song is matched with root's or till node->next is null
    while(iter-> next != NULL && (strcmp(iter->next->songName,songName) != 0)){
		//move to the next node
	    iter = iter -> next;
    }
	//if song is not found at the end of the list print a message
    if(iter -> next == NULL){
        printf("song is not found..\n");
		return root;
    }
    temp = iter->next; // node to delete 
    iter->next = iter->next->next; //jump into the next node 
	//print the message with song that will be deleted
	printf("The song '%s' is deleted from the list..\n",temp->songName);
	//delete the song in the temp node
    free(temp);
	printf("\n");
	//return the head pointer
    return root;
}
/*
*deleting with index likely delete method
*/
struct node * deleteWithIndex(struct node * root, int index){
	struct node * temp,*iter = root; int counter = 0;
	if(iter == NULL) return root;
	if(index < 0){
		//printf("Please enter a positive index..\n");
		return root;
	}
	if(index == 0){
		temp = iter;
		iter = iter->next;
		//printf("The song '%s' is deleted from the list..\n",temp->songName);
		free(temp);
		return iter;
	}
	while(iter -> next != NULL && counter < index-1){
		iter = iter->next;
		counter++;
	}
	temp = iter->next;
	iter->next = iter->next->next;
	free(temp);
	return root;
}
//printList method to print the content of the linked list
void printList(struct node * root){
	//take a head pointer as parameter that will be printed and assign it to iter
	struct node * iter = root;
	// static int variables to the print titles of the lists
	switch(iter->pointer){ 
		case 1: printf("The list in chronological order:\n");break;
		case 2: printf("The list in duration-time order:\n");break;
		case 3: printf("The list in alphabetical order:\n");break;
		case 4: printf("The list in random order:\n");break;
	}
	int i = 1;
	//move to the end of the list
    while(iter != NULL){
		//seperate the duration as minute and second
    	int minute = iter->duration / 60;
		int second = iter->duration - minute*60;
		//print the appropriate message to the screen for each node
		if(second < 10)
			printf("%d.%s %d:0%d\n",i,iter->songName,minute,second);
        else printf("%d.%s %d:%d\n",i,iter->songName,minute,second);
		//move to the next node
        iter = iter-> next;
        i++;
    }
	printf("\n"); 
}
//clean the string nameBuffer totally
void nameBufferCleaner(char *nameBuffer){
	for(int i = 0;i< strlen(nameBuffer);i++){
		if(nameBuffer[i] != ' '){
		//all characters of nameBuffer is ' '
		nameBuffer[i] = ' ';
		}
	}
}
//deleting the empty spaces in string(not the ones between words)
void nameBufferUpdater(char *nameBuffer){
    int i = 0,j;
	//to control one character and the next character
    for(j = i+1; j < strlen(nameBuffer); i++,j++){
			//if both are emtpy space, delete them
			if(nameBuffer[i] == ' ' && nameBuffer[j] == ' '){
				nameBuffer[i] = '\0';nameBuffer[j] = '\0';
			}
			//"string " situation, control last space and delete it
			if(j == strlen(nameBuffer)-1 && nameBuffer[j] == ' '){
				nameBuffer[j] = '\0';
			}
		}
}
//return the length of the given pointer
int length(struct node * root){
	int i = 0;
	//move to the end of the list
	while(root != NULL){
		root = root-> next;
		//increase i by 1
		i++;
	}
	//return i as length of the list
	return i;	
}
//sort the list chronological order
struct node * sortChronoOrder(struct node * root){
	struct node * iter = root;
	//create a new list and copy the list to it and return
	//for list is already chronologic, so just copy it and return 
	struct node * chrono_head = NULL;
	while(iter != NULL){
		chrono_head = insertNode(chrono_head,iter->songName,iter->duration);
		iter = iter->next;
	}
	//assign the pointer for printing message
	chrono_head->pointer = chrono;
	return chrono_head;	
}
//sort the list according to durations of songs
struct node * sortDuration_TimeOrder(struct node *root) { 
	//iter for given pointer
	struct node * iter = root;
	//and duration_head is new pointer
	struct node * duration_head = NULL;
    int isSwapped; 
	//defining the pointers that will be isSwapped
    struct node *ptr1,*ptr2 = NULL;  
    if (root == NULL) 
		//if root is null return as it is
        return root; 
    do{ //assign isSwapped 0 and return till the isSwapped is not a zero
        isSwapped = 0; 
		//assign the ptr1 to roo
        ptr1 = root; 
		//not side by side loop
        while (ptr1->next != ptr2) {
			//check the durations and swap them 
            if (ptr1->duration > ptr1->next->duration) {  
                swap(ptr1, ptr1->next); 
				//no more returns 
                isSwapped = 1; 
            } 
			//duration is not bigger of the next's , move to the next
            ptr1 = ptr1->next; 
        } 
		//assign the Null pointer to ptr1 to compare truly in inner while
        ptr2 = ptr1; 
    }while (isSwapped);
	
	//iter is correctly listed now, print iter to the duration_head pointer and return it 
	while(iter != NULL){
			duration_head = insertNode(duration_head,iter->songName,iter->duration);
			iter = iter->next;
		}
		//assign the pointer for printing message
		duration_head->pointer = durate;
		return duration_head;
} 
/*
*alpha order is the same logic with sortDuration but this time we compare to 
strings' ascii values character by character to sort them alphabetically
*/
struct node * sortAlphaOrder(struct node * root){
	struct node * iter = root;
	struct node * alpha_head = NULL;
		int isSwapped, i = 0; 
		struct node *nodePtr1,*nodePtr2 = NULL;  
		if (root == NULL) 
			return root; 
		do{ 
			isSwapped = 0; 
			nodePtr1 = root; 
			while (nodePtr1->next != nodePtr2) { 
				if (nodePtr1->songName[i] > nodePtr1->next->songName[i]) {  
					swap(nodePtr1, nodePtr1->next); 
					isSwapped = 1; 
				} 
				nodePtr1 = nodePtr1->next; 
			} 
			nodePtr2 = nodePtr1; 
		}while (isSwapped); 
		//again copy the given root to alpha_head and return it
		while(iter != NULL){
			alpha_head = insertNode(alpha_head,iter->songName,iter->duration);
			iter = iter->next;
		}
		alpha_head->pointer = alpha;
		return alpha_head;
}
//sort random the list
struct node * sortRandomOrder(node * root){
	//return node
	struct node * random_head = NULL;
	//define an array for random numbers
	int arr[length(root)];
	for(int i = 0; i < length(root); i++){
		//give the numbers till 8 
		arr[i] = i;
	}
	//and shuffle the numbers with method randomize 
	randomize(arr,length(root));
	for(int i = 0; i < length(root); i++){
		struct node * iter = root;
		int counter = 0;
		//take the random numberth of the list node
		while(counter < arr[i]){
			iter = iter->next;
			counter++;
		}
		//add to new list random_head
		random_head = insertNode(random_head,iter->songName,iter->duration);
	}
	//assign the pointer value
	random_head->pointer = randomy;
	//return the new list random_head
	return random_head;
}
//swapping node's songName and duration properties
void swap(struct node *ptr1,struct node *ptr2){
	int temp; 						 char copy [25] = {'\0'};
	temp = ptr1->duration;   		 strcpy(copy,ptr1->songName);
 	ptr1->duration = ptr2->duration; strcpy(ptr1->songName,ptr2->songName);
	ptr2->duration = temp;			 strcpy(ptr2->songName,copy);
}
//swap  integer using int pointer in array to randomize it
void swapInts(int *a, int *b) { 
	int temp = *a; 
	*a = *b; 
	*b = temp; 
} 
//array and length of the array parameters
void randomize (int arr[],int n) { 
	//to do not repeat the same random number
	srand(time(NULL)); 
	for (int i = n-1; i>0; i--) { 
		//take a random value 
		int j = rand() % (i+1);
		//swap the places  
		swapInts(&arr[i], &arr[j]);
	} 
} 
/* 
*take four head pointers and update them with methods of them and print the lists one by one
*/ 
void updateAllAndPrint(struct node * chrono_head,struct node * duration_head,
					   struct node * alpha_head, struct node * random_head){
	chrono_head = sortChronoOrder(chrono_head);
	printList(chrono_head);
	alpha_head = sortAlphaOrder(chrono_head);
	printList(alpha_head);
	duration_head = sortDuration_TimeOrder(chrono_head);
	printList(duration_head);
	random_head = sortRandomOrder(chrono_head);
	printList(random_head);		   
}
void writerFunc(node * root, FILE * outFile){
		struct node * iter = root;
		//according to pointer values of lists, print the title to the file
		switch(iter->pointer){
		case 1: fprintf(outFile,"The list in chronological order:\n");break;
		case 2: fprintf(outFile,"The list in duration-time order:\n");break;
		case 3: fprintf(outFile,"The list in alphabetical order:\n");break;
		case 4: fprintf(outFile,"The list in random order:\n");break;
	}
		//move inside the list and print the properties appropriately to the file
		int i = 1;
		while(iter!= NULL){
			int minute,second;
			minute = iter->duration/60;
			second = iter->duration - minute*60;
			fprintf(outFile,"%d.%s %d:%d\n",i,iter->songName,minute,second);
			iter = iter->next;
			i++;
		}
		fprintf(outFile,"\n");
}
