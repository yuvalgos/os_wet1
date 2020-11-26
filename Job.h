class Job 
{
public:
	std::string name;
	int pid;
	std::chrono::system_clock::time_point insertion_time;
	bool isStopped;

	Job(std::string name, int pid, bool isStopped = false) :
	 name(name), pid(pid), insertion_time(std::chrono::system_clock::now()),
	 isStopped(isStopped) {}; 

    Job(const Job& j) :
	 name(j.name), pid(j.pid), insertion_time(j.insertion_time),
	 isStopped(j.isStopped) {}; 

    Job(): name(""), pid(-1), insertion_time(std::chrono::system_clock::now()),
    isStopped(false){};

	int getTime()
	{
		return std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now() - insertion_time).count();
	}
};