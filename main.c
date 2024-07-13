#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char course [100];
    int creditHours;
    char courseCode [100];
    char department [100];
    char topic [200];

} Courses;
///////////////////////////
struct AVLnode;
typedef struct AVLnode *AVL_T;

struct AVLnode
{
    char AVL_course [100];
    int AVL_hours;
    char AVL_code[100];
    char AVL_department[100];
    char AVL_topic [200];
    AVL_T Left;
    AVL_T Right;
    int Height;
};
////////////////////////////
int menuData() // function to display the menu to the user
{
    printf("======================MENU=========================\n");
    printf("1) Read the file courses.txt file and create the tree\n");
    printf("2) Insert a new course from the user with all its associated data\n");
    printf("3) Find a course and support updating of its information\n");
    printf("4) List courses in lexicographic order with their associated information (credit hours, IDs, and topics)\n");
    printf("5) List all topics associated with a given course\n");
    printf("6) List all courses in lexicographic order that belong to the same department\n");
    printf("7) Delete a course\n");
    printf("8) Delete all courses that start with a specific letter\n");
    printf("9) Delete all courses belong to a given department\n");
    printf("10) Save all words in file offered_courses.txt\n");
    printf("11) Exit program\n");
    printf("===================================================\n");
    printf("Enter the case you want to process:\n");
    int x;
    scanf("%d", &x);
    return x;
}
////////////////////////////
int getHeight (AVL_T t)
{
    if(t==NULL)
        return -1;
    else
        return t->Height;
}
/////////////////////////////
int MAX (int x, int y)
{
    if(x>y)
        return x;
    else
        return y;
}
////////////////////////////////////////////////

AVL_T FindMin( AVL_T T )
{
    if( T == NULL )
        return NULL;
    else if( T->Left == NULL )
        return T;
    else
        return FindMin( T->Left );
}
////////////////////////////////////////////////
AVL_T FindMax( AVL_T T )
{
    if( T != NULL )
        while( T->Right != NULL )
            T = T->Right;

    return T;
}
////////////////////////////////////////////////
AVL_T singleRotateLeft(AVL_T x)
{
    AVL_T y;
    y         = x->Right;
    x->Right  = y->Left;
    y->Left = x;

    x->Height = 1 + MAX(getHeight(x->Left), getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x), getHeight(y->Right));

    return y;
}
////////////////////////////////////////////////
AVL_T singleRotateRight(AVL_T y)
{
    AVL_T x;
    x        = y->Left;
    y->Left  = x->Right;
    x->Right = y;

    y->Height = 1 + MAX(getHeight(y->Right), getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y), getHeight(x->Left));

    return x;
}
////////////////////////////////////////////////
AVL_T doubleRotateLeft(AVL_T K3)
{
    K3->Left  = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}
////////////////////////////////////////////////
AVL_T doubleRotateRight(AVL_T K3)
{
    K3->Right  = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}
////////////////////////////////////////////////
int getBalance(AVL_T t)
{
    if (t == NULL)
        return 0;
    return getHeight(t->Left) - getHeight(t->Right);
}
////////////////////////////////////////////////
AVL_T insert(AVL_T t, char course[100], int hour,char code[100],char department[100],char topic[200])
{
    if(t==NULL)
    {
        t= (AVL_T)malloc(sizeof (struct AVLnode));
        strcpy(t->AVL_course, course);
        t->AVL_hours = hour;
        strcpy(t->AVL_code, code);
        strcpy(t->AVL_department, department);
        strcpy(t->AVL_topic, topic);
        t->Height=0;
        t->Left =t->Right= NULL;

    }
    else if(strcmp(course, t->AVL_course)<0)
    {
        t->Left= insert(t-> Left, course, hour, code, department, topic);
        if (getHeight(t->Left)- getHeight(t->Right)>=2)
        {
            if(strcmp(course,t->Left->AVL_course)<0)
                t = singleRotateLeft(t);
        }
        else
            t=doubleRotateLeft(t);
    }
    else if (strcmp(course, t->AVL_course)>0)
    {
        t->Right= insert(t-> Right, course, hour, code, department, topic);
        if(getHeight(t->Right)- getHeight(t->Left)>=2)
        {
            if (strcmp(course, t->Right->AVL_course)>0)
                t= singleRotateLeft(t);
        }
        else
            t= doubleRotateRight(t);
    }
    t->Height= MAX(getHeight(t->Right), getHeight(t->Left))+1;
    return t;
}
void printInOrder(AVL_T t)
{
    if(t!= NULL)
    {
        printInOrder(t->Left);
        printf("%-30s || %-10d || %-30s || %-25s \n", t->AVL_course, t->AVL_hours, t->AVL_code,t->AVL_department);
        printf("Topics: %s\n",t->AVL_topic);
        printInOrder(t->Right);
    }
}
////////////////////////////////////////////////////////////////////////////
void printLexicographicOrder(AVL_T t)
{
    if (t != NULL)
    {
        printInOrder(t->Left);
        printf("%-10d || %-30s\n", t->AVL_hours, t->AVL_code);
        printf("Subjects: %s\n",t->AVL_topic);
        printInOrder(t->Right);

    }
}
////////////////////////////////////////////////////////////////////////////
AVL_T Delete(char key[100], AVL_T t)
{
    if (t == NULL)
        return t;

    if ( strcmp(key, t->AVL_course )<0)
        t->Left = Delete(key, t->Left);

    else if( strcmp(key, t->AVL_course)>0)
        t->Right = Delete(key, t->Right);

    else
    {
        if( (t->Left == NULL) || (t->Right == NULL) )
        {
            AVL_T temp = t->Left ? t->Left : t->Right;

            if (temp == NULL)
            {
                temp = t;
                t = NULL;
            }
            else
                *t = *temp;
            free(temp);
        }
        else
        {
            AVL_T temp = FindMin(t->Right);

            strcpy(t->AVL_course, temp->AVL_course ) ;

            t->Right = Delete(temp->AVL_course, t->Right );
        }
    }

    if (t == NULL)
        return t;

    t->Height = 1 + MAX(getHeight(t->Left),getHeight(t->Right));

    int balance = getBalance(t);

    if (balance > 1 && getBalance(t->Left) >= 0)
        return singleRotateRight(t);

    if (balance > 1 && getBalance(t->Left) < 0)
    {
        t->Left =  singleRotateLeft(t->Left);
        return singleRotateRight(t);
    }

    if (balance < -1 && getBalance(t->Right) <= 0)
        return singleRotateLeft(t);

    if (balance < -1 && getBalance(t->Right) > 0)
    {
        t->Right = singleRotateRight(t->Right);
        return singleRotateLeft(t);
    }
    return t;
}
////////////////////////////////////////////////////////
AVL_T deleteFromTree(AVL_T t, char c)
{
    int search;
    AVL_T tree= NULL;
    if (t == NULL)
        return t;
    else
    {
        search = t->AVL_course[0] - c;
        if (search == 0)
        {
            tree = Delete(t->AVL_course, t);
            return deleteFromTree(t, c);
        }
        else if (search > 0)
        {
            tree = deleteFromTree(t->Left, c);
        }
        else
        {
            tree =  deleteFromTree(t->Right, c);
        }
    }
    return t;

}
////////////////////////////////////////////////////////
AVL_T findNode(AVL_T t, char key[100])
{
    if (t == NULL)
        return NULL;
    else if (strcmp(t ->AVL_course, key)<0)
        return findNode(t ->Left, key);
    else if (strcmp(t ->AVL_course, key)>0)
        return findNode(t ->Right, key);
    else
        return t;

}
////////////////////////////////////////////////////////
AVL_T MakeEmpty(AVL_T T)
{
    if(T!=NULL)
    {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}
////////////////////////////////////////////////////////
/*printToFile(AVL_T t, FILE out)
{
    if(t!= NULL)
    {
        printInOrder(t->Left);
        fprintf(out, "%-30s || %-10d || %-30s || %-25s \n", t->AVL_course, t->AVL_hours, t->AVL_code,t->AVL_department);
        fprintf(out, "Topics: %s\n",t->AVL_topic);
        printInOrder(t->Right);
    }
}*/
/////////////////////////////////////////////////////////
int main()
{
    FILE *fin = fopen("courses.txt", "r"); // opening buses file
    int choice;
    char name[100]="";
    int h=0;
    char code[100]="";
    char department[100]="";
    char topic[200]="";

    do
    {
        choice = menuData();
            switch (choice) // switch to ask the user to enter which part to execute
            {
            case 1:
                if (fin == NULL)
                {
                    printf("File does not exist");
                }
                AVL_T root;
                root = MakeEmpty(NULL);
                Courses c[200];
                char arr[200];
                char * field;
                int i=0;
                while (fgets(arr,200,fin)!=NULL) // reading the buses file and dividing data depending on # using strtok
                {
                    field = strtok(arr,":");
                    strcpy(c[i].course,field);

                    field = strtok(NULL,"#");
                    c[i].creditHours = atoi(field);

                    field = strtok(NULL,"#");
                    strcpy(c[i].courseCode,field);

                    field = strtok(NULL,"/");
                    strcpy(c[i].department,field);

                    field = strtok(NULL, "\n");
                    strcpy(c[i].topic, field);
                    root =insert(root, c[i].course, c[i].creditHours, c[i].courseCode, c[i].department, c[i].topic);
                    i++;
                }
                printf("---------------- print Tree elements in IN-order-------------\n\n");
                printInOrder(root);
                break;

                char name[100]="";
                int h=0;
                char code[100]="";
                char department[100]="";
                char topic[200]="";
            case 2:
                printf("Enter a new course with all its data to insert to the tree:\n");
                scanf("%s", name);
                scanf("%d", &h);
                scanf("%s", code);
                scanf("%s", department);
                scanf("%s", topic);

                root = insert(root, name, h, code, department, topic);
                break;
                char cName[100]="";
                int cHour=0;
                char cCode[100]="";
                char cDepartment [100]="";
                char cTopic[200]="";
            case 3:
                printf("Enter the course to find and update information:\n");
                scanf("%s", cName);
                AVL_T tree= findNode(root, cName);
                printf("Enter the data values to update: (course name, hour, code, department, topic)\n");
                scanf("%s", cName);
                scanf("%d", &cHour);
                scanf("%s", cCode);
                scanf("%s", cDepartment);
                scanf("%s", cTopic);

                if (tree == NULL)
                    printf("Course not found\n");
                else
                {
                    strcpy(tree->AVL_course, cName);
                    tree->AVL_hours = cHour;
                    strcpy(tree->AVL_code, cCode);
                    strcpy(tree->AVL_department, cDepartment);
                    strcpy(tree->AVL_topic, cTopic);
                    printf ("The data is successfully updated\n");
                }
                break;
            case 4:
                printf("The courses in in lexicographic order:\n");
                printLexicographicOrder(root);
                break;

                char courseName[100]="";
            case 5:
                printf("Enter the course to list the related topics:\n");
                scanf("%s", courseName);
                AVL_T r= findNode(root, courseName);
                if (r == NULL)
                    printf("Course not found\n");
                else
                {
                    printf("The topics of %s course are:\n", courseName);
                    printf("%s", r->AVL_topic);
                }
                break;
                char departmentName [100]="";
            case 6:
                printf("Enter the department name to delete all courses related:\n");
                scanf("%s", departmentName);


                break;
                char courseN[100]="";
            case 7:
                printf("Enter the course code you want to delete:\n");
                scanf("%s", courseN);
                Delete(courseN, root);
                break;
            case 8:

                break;
                char x;
            case 9:
                printf("Enter the letter to delete all courses that start with it:\n");
                scanf("%c", &x);
                deleteFromTree(root, x);
                break;
            //FILE out = fopen("offered_courses.txt", "w");
            /*case 10:
                if (out == NULL)
                    printf("File does not exist");
                else
                    printToFile( roots, out);
                break;*/
            case 11:
                printf("Exiting the program\n"); // a case to exit the program
                break;
            default:
                printf("Wrong choice try again\n"); // if a different choice is entered then try again
            }
        }
        while (choice !=  11);
}

