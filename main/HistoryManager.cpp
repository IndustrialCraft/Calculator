#include "HistoryManager.hpp"

HistoryManager::HistoryManager() {}

void HistoryManager::addHistory(std::string str) {
    m_history.push_front(str);
    if (m_history.size() > 10)
        m_history.pop_back();
}
const std::deque<std::string>& HistoryManager::getHistory() {
    return m_history;
}