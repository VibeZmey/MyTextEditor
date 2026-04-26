#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <string>
#include <vector>

class SearchObserver {
public:
    virtual ~SearchObserver() = default;
    virtual void onSearchResultsChanged(const std::vector<int>& results) = 0;
};

class SearchManager {
private:
    std::vector<SearchObserver*> observers;
    std::string lastQuery;
    std::vector<int> lastResults;
    bool caseSensitive;

public:
    SearchManager();
    ~SearchManager();

    void subscribe(SearchObserver* observer);
    void unsubscribe(SearchObserver* observer);

    std::vector<int> search(const std::string& text, const std::string& query);
    std::vector<int> searchCaseSensitive(const std::string& text, const std::string& query);

    void setCaseSensitive(bool sensitive);
    bool isCaseSensitive() const;

    std::vector<int> getLastResults() const;
    std::string getLastQuery() const;
    int getResultCount() const;

private:
    void notifyObservers();
};

#endif // SEARCHMANAGER_H
