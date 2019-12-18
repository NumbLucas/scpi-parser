#include <string>
#include <vector>
using namespace std;

int handleMultiCmd(char *);
void splitStr(const string& str, const char& separator, vector<string>& rst);
void splitCmdAndParm(const string& str,string& cmd, string& param);
void removeHeadTailSpace(string& str);