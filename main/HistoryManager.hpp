#pragma once

#include <deque>
#include <string>

class HistoryManager {
public:
    std::deque<std::string> m_history;
public:
    void addHistory(std::string str);
    const std::deque<std::string>& getHistory();
public:
    static HistoryManager& getInstance() {
        static HistoryManager instance;
        return instance;
    }
private:
    HistoryManager();
public:
    HistoryManager(HistoryManager const&) = delete;
    void operator=(HistoryManager const&) = delete;
};