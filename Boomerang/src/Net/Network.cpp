#include "Network.h"

#include "Util/ThreadPool.h"


namespace Boomerang
{
	std::shared_ptr<std::vector<std::string>> Network::AwaitMessageForClients()
	{
		ThreadPool<std::string> thread_pool((int)m_Clients.size());
		for(auto i = m_Clients.begin(); i != m_Clients.end(); ++i)
		{
			int client_id = i->second->id;
			std::function<std::string()> task = [this, client_id]() -> std::string {
				return this->AwaitClient(client_id);
			};
			thread_pool.submit_task(task);
		}

		std::shared_ptr<std::vector<std::string>> results = thread_pool.run_tasks();

		return results;
	}

}
