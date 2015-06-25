#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class TimeLogger{
	private:
		ofstream *file;
		int logsCount;
	public:
		TimeLogger::TimeLogger(string filename ){
			this->file = new ofstream(filename);
			this->logsCount = 0;
		}
		void TimeLogger::insertNewLine(double time){
			if (this->logsCount<10000){
			*file << time << endl;
			logsCount++;
			}
		}
		TimeLogger::~TimeLogger(){
			this->file->close();
		}
};