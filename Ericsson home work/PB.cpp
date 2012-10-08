// PhoneBook.cpp : 定义控制台应用程序的入口点。
//说明：本工程有两个函数查找带？和*的内容，来源于互联网
//
#include "stdio.h"
#include "tchar.h"
#include "iostream"
#include "stdlib.h"

#include "string.h"

using namespace std;

//Access phone book method
typedef enum __ACCESS_TYPE_T__
{
	ACCT_NAME = 0, //Access phone book linkman by name
	ACCT_MOBILE, //by mobile phone number
	ACCT_ADDRESS, //by person address
	ACCT_COUNT
}access_type;

const char *sel_access_method_str = "by (name|mobile|address):";
const char access_method_infor[3][10] =
{
	"name", "mobile", "address"
};

//get access type by input string
access_type get_access_method(char sel_str[])
{
	access_type acct = ACCT_COUNT;

	for (int i = 0; i < ACCT_COUNT; i++)
	{
		if (strcmp(access_method_infor[i], sel_str) == 0)
		{
			acct = (access_type)i;
			break;
		}
	}
	return acct;
}

//calss of linkman information
typedef struct __LINKMAN_T__
{
	char *name;
	char *mobile;
	char *address;
	__LINKMAN_T__ *next;
}linkman;

static void show_linkman_title();
static void show_linkman(int num, linkman *lm_item);

//-->来自互联网（开始）
//功  能：在lpszSour中查找字符串lpszFind，lpszFind中可以包含通配字符‘?’
//参  数：nStart为在lpszSour中的起始查找位置
//返回值：成功返回匹配位置，否则返回-1
//注  意：Called by “bool MatchingString()”
int FindingString(const char* lpszSour, const char* lpszFind, int nStart /* = 0 */)
{
//	ASSERT(lpszSour && lpszFind && nStart >= 0);
	if(lpszSour == NULL || lpszFind == NULL || nStart < 0)
		return -1;

	int m = strlen(lpszSour);
	int n = strlen(lpszFind);

	if( nStart+n > m )
		return -1;

	if(n == 0)
		return nStart;

//KMP算法
	int* next = new int[n];
	//得到查找字符串的next数组
	{	n--;

		int j, k;
		j = 0;
		k = -1;
		next[0] = -1;

		while(j < n)
		{	if(k == -1 || lpszFind[k] == '?' || lpszFind[j] == lpszFind[k])
			{	j++;
				k++;
				next[j] = k;
			}
			else
				k = next[k];
		}

		n++;
	}

	int i = nStart, j = 0;
	while(i < m && j < n)
	{
		if(j == -1 || lpszFind[j] == '?' || lpszSour[i] == lpszFind[j])
		{	i++;
			j++;
		}
		else
			j = next[j];
	}

	delete []next;

	if(j >= n)
		return i-n;
	else
		return -1;
}

//功	  能：带通配符的字符串匹配
//参	  数：lpszSour是一个普通字符串；
//			  lpszMatch是一可以包含通配符的字符串；
//			  bMatchCase为0，不区分大小写，否则区分大小写。
//返  回  值：匹配，返回1；否则返回0。
//通配符意义：
//		‘*’	代表任意字符串，包括空字符串；
//		‘?’	代表任意一个字符，不能为空；
//时	  间：	2001.11.02	13:00
bool MatchingString(const char* lpszSour, const char* lpszMatch, bool bMatchCase /*  = true */)
{
//	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszMatch));
	if(lpszSour == NULL || lpszMatch == NULL)
		return false;

	if(lpszMatch[0] == 0)//Is a empty string
	{
		if(lpszSour[0] == 0)
			return true;
		else
			return false;
	}

	int i = 0, j = 0;

	//生成比较用临时源字符串'szSource'
	char* szSource =
		new char[ (j = strlen(lpszSour)+1) ];

	if( bMatchCase )
	{	//memcpy(szSource, lpszSour, j);
		while( *(szSource+i) = *(lpszSour+i++) );
	}
	else
	{	//Lowercase 'lpszSour' to 'szSource'
		i = 0;
		while(lpszSour[i])
		{	if(lpszSour[i] >= 'A' && lpszSour[i] <= 'Z')
				szSource[i] = lpszSour[i] - 'A' + 'a';
			else
				szSource[i] = lpszSour[i];

			i++;
		}
		szSource[i] = 0;
	}

	//生成比较用临时匹配字符串'szMatcher'
	char* szMatcher = new char[strlen(lpszMatch)+1];

	//把lpszMatch里面连续的“*”并成一个“*”后复制到szMatcher中
	i = j = 0;
	while(lpszMatch[i])
	{
		szMatcher[j++] = (!bMatchCase) ?
								( (lpszMatch[i] >= 'A' && lpszMatch[i] <= 'Z') ?//Lowercase lpszMatch[i] to szMatcher[j]
										lpszMatch[i] - 'A' + 'a' :
										lpszMatch[i]
								) :
								lpszMatch[i];		 //Copy lpszMatch[i] to szMatcher[j]
		//Merge '*'
		if(lpszMatch[i] == '*')
			while(lpszMatch[++i] == '*');
		else
			i++;
	}
	szMatcher[j] = 0;

	//开始进行匹配检查

	int nMatchOffset, nSourOffset;

	bool bIsMatched = true;
	nMatchOffset = nSourOffset = 0;
	while(szMatcher[nMatchOffset])
	{
		if(szMatcher[nMatchOffset] == '*')
		{
			if(szMatcher[nMatchOffset+1] == 0)
			{	//szMatcher[nMatchOffset]是最后一个字符

				bIsMatched = true;
				break;
			}
			else
			{	//szMatcher[nMatchOffset+1]只能是'?'或普通字符

				int nSubOffset = nMatchOffset+1;

				while(szMatcher[nSubOffset])
				{	if(szMatcher[nSubOffset] == '*')
						break;
					nSubOffset++;
				}

				if( strlen(szSource+nSourOffset) <
						size_t(nSubOffset-nMatchOffset-1) )
				{	//源字符串剩下的长度小于匹配串剩下要求长度
					bIsMatched = false; //判定不匹配
					break;			//退出
				}

				if(!szMatcher[nSubOffset])//nSubOffset is point to ender of 'szMatcher'
				{	//检查剩下部分字符是否一一匹配

					nSubOffset--;
					int nTempSourOffset = strlen(szSource)-1;
					//从后向前进行匹配
					while(szMatcher[nSubOffset] != '*')
					{
						if(szMatcher[nSubOffset] == '?')
							;
						else
						{	if(szMatcher[nSubOffset] != szSource[nTempSourOffset])
							{	bIsMatched = false;
								break;
							}
						}
						nSubOffset--;
						nTempSourOffset--;
					}
					break;
				}
				else//szMatcher[nSubOffset] == '*'
				{	nSubOffset -= nMatchOffset;

					char* szTempFinder = new char[nSubOffset];
					nSubOffset--;
					memcpy(szTempFinder, szMatcher+nMatchOffset+1, nSubOffset);
					szTempFinder[nSubOffset] = 0;

					int nPos = ::FindingString(szSource+nSourOffset, szTempFinder, 0);
					delete []szTempFinder;

					if(nPos != -1)//在'szSource+nSourOffset'中找到szTempFinder
					{	nMatchOffset += nSubOffset;
						nSourOffset += (nPos+nSubOffset-1);
					}
					else
					{	bIsMatched = false;
						break;
					}
				}
			}
		}		//end of "if(szMatcher[nMatchOffset] == '*')"
		else if(szMatcher[nMatchOffset] == '?')
		{
			if(!szSource[nSourOffset])
			{	bIsMatched = false;
				break;
			}
			if(!szMatcher[nMatchOffset+1] && szSource[nSourOffset+1])
			{	//如果szMatcher[nMatchOffset]是最后一个字符，
				//且szSource[nSourOffset]不是最后一个字符
				bIsMatched = false;
				break;
			}
			nMatchOffset++;
			nSourOffset++;
		}
		else//szMatcher[nMatchOffset]为常规字符
		{
			if(szSource[nSourOffset] != szMatcher[nMatchOffset])
			{	bIsMatched = false;
				break;
			}
			if(!szMatcher[nMatchOffset+1] && szSource[nSourOffset+1])
			{	bIsMatched = false;
				break;
			}
			nMatchOffset++;
			nSourOffset++;
		}
	}

	delete []szSource;
	delete []szMatcher;
	return bIsMatched;
}
//<--来自互联网（结束）

/*check the match_context is match source_context or not. eg. 
source_context        match_context   match
test_source           test            No (return false)
test_source           test_source_L   No
test_source           test*           Yes(return true)
test_source           test?source     Yes
test_source           test_source     Yes
*/
bool str_match(char *source_context, char *match_context)
{
	return MatchingString(source_context, match_context, true);
}

class linkman_management
{
public:
	linkman *head;
	int count;

	linkman_management()
	{
		head = NULL;
		count = 0;
		cur_access_lm = NULL;
	}

	//add new linkman to phone book if the linkman is not exist(check name) else overide it.
	void add(linkman new_linkman)
	{
		linkman *lm_item = head; //a linkman point for recursion the phone book(all linkmans)
		linkman *lm_last = NULL; //recoder the insert location

		if (head == NULL)
		{
			//malloc new space
			cur_access_lm = (linkman *)(new char[sizeof(linkman)]);
			//copy data to new linkman buffer space
			cur_access_lm->name = new_linkman.name;
			cur_access_lm->mobile = new_linkman.mobile;
			cur_access_lm->address = new_linkman.address;
			cur_access_lm->next = NULL; //only 1 item(linkman)
			head = cur_access_lm;
			count = 1;
		}
		else
		{
			while(lm_item != NULL)
			{
				if (strcmp(lm_item->name, new_linkman.name) == 0)
				{
					//the linkman is aready exist, overide the old information
					lm_item->mobile = new_linkman.mobile;
					lm_item->address = new_linkman.address;
					return;
				}
				else if (strcmp(lm_item->name, new_linkman.name) < 0)
				{
					lm_last = lm_item;
				}
				else if (strcmp(lm_item->name, new_linkman.name) > 0)
				{
					break;
				}
				lm_item = lm_item->next;
			}
			//malloc new space
			cur_access_lm = (linkman *)(new char[sizeof(linkman)]);
			//copy data to new linkman buffer space
			cur_access_lm->name = new_linkman.name;
			cur_access_lm->mobile = new_linkman.mobile;
			cur_access_lm->address = new_linkman.address;
			cur_access_lm->next = NULL; //initial to empty
			//add new item(by char increase)
			if (lm_last == NULL)
			{
				lm_last = cur_access_lm;
				lm_last->next = head;
				head = lm_last;
			}
			else if (lm_last->next != NULL)
			{
				cur_access_lm->next = lm_last->next;
				lm_last->next = cur_access_lm;
			}
			else
			{
				lm_last->next = cur_access_lm;
			}
			count++;
		}
	}

	//search linkman by name, mobile phone or address
	//The matched items information will be auto show in the Console.
	//The matched items count will be return.
	int search_linkman(char *del_context, access_type acc_type)
	{
		//search linkman and delete it if exist
		linkman *lm_item = head; //a linkman point for recursion the phone book(all linkmans)
		char *search_str; //the linkman of phone book context for delete
		int match_count = 0;
		bool show_title = false;

		while(lm_item != NULL)
		{
			switch(acc_type)
			{
			case ACCT_NAME:
				search_str = lm_item->name;
				break;
			case ACCT_MOBILE:
				search_str = lm_item->mobile;
				break;
			case ACCT_ADDRESS:
				search_str = lm_item->address;
				break;
			default:
				return 0;
			}
			if (str_match(search_str, del_context))
			{
				//find a match item
				if (show_title == false)
				{
					show_title = true;
					show_linkman_title();
				}
			    show_linkman(++match_count, lm_item);
			}
			lm_item = lm_item->next;
		}
		if (match_count > 0)
		{
			printf("\r\n");
		}
		return match_count;
	}

	//delete the linkman by name, mobile phone or address, and return the deleted items count
	int delete_linkman(char *del_context, access_type acc_type)
	{		
		//search linkman and delete it if exist
		linkman *lm_last = NULL; //2 linkman point for recursion the phone book(all linkmans)
		linkman *lm_item = head;
		linkman *lm_delete = NULL; //point the delete item
		char *search_str; //the linkman of phone book context for delete
		int del_count = 0;

		while(lm_item != NULL)
		{
			switch(acc_type)
			{
			case ACCT_NAME:
				search_str = lm_item->name;
				break;
			case ACCT_MOBILE:
				search_str = lm_item->mobile;
				break;
			case ACCT_ADDRESS:
				search_str = lm_item->address;
				break;
			default:
				return 0;
			}
			lm_delete = NULL;
			if (str_match(search_str, del_context))
			{
				//find a match item
				lm_delete = lm_item;
				count--;
				del_count++;
				if (lm_last == NULL)
				{
					head = lm_item->next;
				}
				else
				{
					lm_last->next = lm_item->next;
				}
			}
			else
			{
			    lm_last = lm_item;
			}
			lm_item = lm_item->next;
			if (NULL != lm_delete)
			{
				delete(lm_delete);
			}
		}
		return del_count;
	}

	//check the linkman is exist or not
	bool check_linkman(linkman new_linkman)
	{
		//check the linkman name is exist or not
		linkman *lm_item = head; //a linkman point for recursion the phone book(all linkmans)
		while(lm_item != NULL)
		{
			if (strcmp(lm_item->name, new_linkman.name) == 0)
			{
				//the linkman is aready exist
				return true;
			}
			else
			{
				lm_item = lm_item->next;
			}
		}
		return false;
	}
private:
	linkman *cur_access_lm;
};

typedef struct __PROCESS_FUNCTION_T__
{
	char *pro_name;
	void (*pro_func)();
}process_function;

static void add_new_link();
static void search_link();
static void delete_link();
static void show_help();
static void show_all();

static void save_phone_book();

const char *quit_id = "!quit";

const process_function pro_func_list[] = 
{
	{"add", &add_new_link},
	{"search", &search_link},
	{"delete", &delete_link},
	{"!help", &show_help},
	{"showall", &show_all}
};

const int pro_funcs = sizeof(pro_func_list) / sizeof(process_function);

linkman_management phone_book;

void add_new_link()
{
	unsigned int i = 0;
	linkman new_lm;
	char *input_infor = new char[400];

	//set name
	_tprintf(_T("Name:"));
	fflush(stdin);
	scanf("%[^\n]",input_infor, 20); //name char max. length: 20 - 1
	for (i = 0; i < strlen(input_infor); i++)
	{
		if (input_infor[i] == ',' || input_infor[i] == '"' || input_infor[i] == '?'  || input_infor[i] == '*')
		{
			_tprintf(_T("Name char , \" ? * is invalid"));
			fflush(stdin);
			return;
		}
	}
	new_lm.name = new char[strlen(input_infor) + 1]; //malloc name buffer
	strcpy(new_lm.name, input_infor);

	//set mobile
	_tprintf(_T("mobile:"));
	fflush(stdin);
	scanf("%[^\n]",input_infor, 20); //mobile char max. length: 20 - 1
	for ( i = 0; i < strlen(input_infor); i++)
	{
		if (input_infor[i] == ',' || input_infor[i] == '"' || input_infor[i] == '?'  || input_infor[i] == '*')
		{
			_tprintf(_T("Mobile char , \" ? * is invalid"));
			fflush(stdin);
			return;
		}
	}
	new_lm.mobile = new char[strlen(input_infor) + 1]; //malloc mobile buffer
	strcpy(new_lm.mobile, input_infor);

	//set address
	_tprintf(_T("address:"));
	fflush(stdin);
	scanf("%[^\n]",input_infor, 400); //address char max. length: 400 - 1
	for (i = 0; i < strlen(input_infor); i++)
	{
		if (input_infor[i] == '?'  || input_infor[i] == '"' || input_infor[i] == '*')
		{
			_tprintf(_T("Address char ? \" * is invalid"));
			fflush(stdin);
			return;
		}
	}
	new_lm.address = new char[strlen(input_infor) + 1]; //malloc address buffer
	strcpy(new_lm.address, input_infor);
	fflush(stdin);
	//check the linkman name
	if (phone_book.check_linkman(new_lm))
	{
		char sel = 'N';
		printf("The name %s was exist(Overide:Y/N):", new_lm.name);
		fflush(stdin); 
		sel = getchar();
		if (sel != 'Y' && sel != 'y')
		{
			_tprintf(_T("address entry ignore!"));
			return;
		}
	}
	phone_book.add(new_lm);
	save_phone_book();
	_tprintf(_T("address entry added!"));
}

void search_link()
{
	char search_inf[400];
	access_type acct = ACCT_COUNT; //record delete linkman method
	int del_count = 0; //record delete item count

	//select delete method
	printf("%s", sel_access_method_str);
	fflush(stdin);
	scanf("%s",search_inf, 10);
	acct = get_access_method(search_inf);
	if (acct < ACCT_COUNT)
	{
		//get delete regular expression
		printf("%s:",access_method_infor[(int)acct]);
		fflush(stdin);
		if (acct < ACCT_ADDRESS)
		{
			scanf("%[^\n]",search_inf, 26);
		}
		else
		{
			scanf("%[^\n]",search_inf, 400);
		}
		fflush(stdin);
		//delete linkman
		del_count = phone_book.search_linkman(search_inf, acct);
		//show delete result
		if (del_count <= 0)
		{
			printf("unfind!");
		}
		else
		{
			printf("%d address entries be find", del_count);
		}
	}
	else
	{
		printf("Unknown access type!");
	}
}

void delete_link()
{
	char del_inf[400];
	access_type acct = ACCT_COUNT; //record delete linkman method
	int del_count = 0; //record delete item count

	//select delete method
	printf("%s", sel_access_method_str);
	fflush(stdin);
	scanf("%s",del_inf, 10);
	acct = get_access_method(del_inf);
	if (acct < ACCT_COUNT)
	{
		//get delete regular expression
		printf("%s:",access_method_infor[(int)acct]);
		fflush(stdin);
		if (acct < ACCT_ADDRESS)
		{
			scanf("%[^\n]",del_inf, 26);
		}
		else
		{
			scanf("%[^\n]",del_inf, 400);
		}
		fflush(stdin);
		//delete linkman
		del_count = phone_book.delete_linkman(del_inf, acct);
		//show delete result
		if (del_count <= 0)
		{
			printf("unfind, delete fail!");
		}
		else
		{
			save_phone_book();
			printf("%d address entries deleted", del_count);
		}
	}
	else
	{
		printf("Unknown access type!");
	}
}

void show_help()
{
	_tprintf(_T("You can used add, search and delete command to manage this phone book!\r\n"));
	_tprintf(_T("add: a new linkman will be add to the phone book.\r\n"));
	_tprintf(_T("search: search a linkman in the phone book.\r\n"));
	_tprintf(_T("delete: delete a linkman in the phone book.\r\n"));
	_tprintf(_T("You can used !quit to exit this application.\r\n"));
	_tprintf(_T("And used showall to see all linkmans of this phone book."));
}

void show_linkman_title()
{
	printf("No.  Name                 Mobile               Address");
}

void show_linkman(int num, linkman *lm_item)
{
	printf("\r\n%-4d %-20s %-20s %s",num, lm_item->name, lm_item->mobile, lm_item->address);
}

void show_all()
{
	linkman *lm_item = phone_book.head; //a linkman point for recursion the phone book(all linkmans)
	int num = 1;

	printf("Phone book have %d linkmans\r\n", phone_book.count);
	if (phone_book.count > 0)
	{
		show_linkman_title();
	}
	while (lm_item != NULL)
	{
		show_linkman(num++, lm_item);
		lm_item = lm_item->next;
	}
}

const char *file_name = "phone_book.csv";
const char *headline = "Name,Mobile,Adress\n";
void load_phone_book()
{
	FILE *fp;
	char file_datas[1024] = "";
	bool remove_headline = false;
	int char_pos;
	int used_chars;
	access_type acct = ACCT_NAME;
	char data_str[400] = {0};
	int data_index = 0;
	int linkman_count = 0;

	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		//do nothing
	}
	else
	{
		linkman lm_new;

		while (feof(fp) == 0)
		{
			used_chars = fread(file_datas, sizeof(char), 1024, fp);
			char_pos = 0;
			data_index = 0;
			if (remove_headline == false)
			{
				remove_headline = true;
				char_pos = strlen(headline);
			}
			for (;char_pos < used_chars; char_pos++)
			{
				switch(file_datas[char_pos])
				{
				case '\n':
					data_str[data_index++] = '\0';
					lm_new.address = (char *)new char[strlen(data_str) + 1];
					strcpy(lm_new.address, data_str);
					data_index = 0;
					phone_book.add(lm_new);
					linkman_count++;
					acct = ACCT_NAME;
					break;
				case ',':
					if (acct == ACCT_NAME)
					{
						data_str[data_index++] = '\0';
						lm_new.name = (char *)new char[strlen(data_str) + 1];
						strcpy(lm_new.name, data_str);
						acct = ACCT_MOBILE;
						data_index = 0;
					}
					else if (acct == ACCT_MOBILE)
					{
						data_str[data_index++] = '\0';
						lm_new.mobile = (char *)new char[strlen(data_str) + 1];
						strcpy(lm_new.mobile, data_str);
						acct = ACCT_ADDRESS;
						data_index = 0;
					}
					else
					{
						data_str[data_index++] = file_datas[char_pos];
					}
					break;
				case '"':
				case '\r':
					break;
				default:
					data_str[data_index++] = file_datas[char_pos];
					break;
				}
			}
		}
		if (data_index > 0)
		{
			data_str[data_index++] = '\0';
			lm_new.address = (char *)new char[strlen(data_str) + 1];
			strcpy(lm_new.address, data_str);
			phone_book.add(lm_new);
			linkman_count++;
		}
		fclose(fp);
	}
	if (linkman_count > 0)
	{
		printf("Phone Book have %d address entries.", linkman_count);
	}
	else
	{
		_tprintf(_T("Phone Book is empty!"));
	}
}

void save_phone_book()
{
	FILE *fp;
	linkman *lm_cur = phone_book.head;

	fp = fopen( file_name, "w");
	fflush(fp);
	fseek(fp, 0, SEEK_SET);
	fwrite(headline, sizeof(char), strlen(headline), fp);

	while (lm_cur != NULL)
	{
		fwrite(lm_cur->name, sizeof(char), strlen(lm_cur->name), fp);
		fwrite(",", sizeof(char), 1, fp);
		fwrite(lm_cur->mobile, sizeof(char), strlen(lm_cur->mobile), fp);
		fwrite(",", sizeof(char), 1, fp);
		fwrite(lm_cur->address, sizeof(char), strlen(lm_cur->address), fp);
		fwrite("\n", sizeof(char), 1, fp);
		lm_cur = lm_cur->next;
	}
	fclose(fp);
}
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int n_ret_code = 0;
	char command[10] = "";

	load_phone_book();

	do
	{
		n_ret_code = 0;
		_tprintf(_T("\r\nPB>"));
		fflush(stdin);
		scanf("%s",command, sizeof(command));
		if (strcmp(command, quit_id) == 0)
		{
			n_ret_code = -1;
		}
		else
		{
			for (int pro_index = 0; pro_index < pro_funcs; pro_index++)
			{
				if (strcmp(pro_func_list[pro_index].pro_name, command) == 0)
				{
					n_ret_code = 1;
					fflush(stdin);
					(*(pro_func_list[pro_index].pro_func))();
				}
			}
			if (0 == n_ret_code)
			{
				_tprintf(_T("Invalid command!\r\n"));
				show_help();
			}
		}
	}while(n_ret_code != -1);
	
	return n_ret_code;
}
