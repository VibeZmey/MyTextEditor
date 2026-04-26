#include "searchmanager.h"
#include <algorithm>
#include <cctype>

SearchManager::SearchManager() : caseSensitive(false) {}

SearchManager::~SearchManager() {}

void SearchManager::subscribe(SearchObserver* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void SearchManager::unsubscribe(SearchObserver* observer) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
        );
}

std::vector<int> SearchManager::search(const std::string& text, const std::string& query) {
    if (caseSensitive) {
        return searchCaseSensitive(text, query);
    }

    lastResults.clear();
    lastQuery = query;

    std::string lowerText = text;
    std::string lowerQuery = query;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    size_t pos = 0;
    while ((pos = lowerText.find(lowerQuery, pos)) != std::string::npos) {
        lastResults.push_back(static_cast<int>(pos));
        pos += lowerQuery.length();
    }

    notifyObservers();
    return lastResults;
}

std::vector<int> SearchManager::searchCaseSensitive(const std::string& text, const std::string& query) {
    lastResults.clear();
    lastQuery = query;

    size_t pos = 0;
    while ((pos = text.find(query, pos)) != std::string::npos) {
        lastResults.push_back(static_cast<int>(pos));
        pos += query.length();
    }

    notifyObservers();
    return lastResults;
}

void SearchManager::setCaseSensitive(bool sensitive) {
    caseSensitive = sensitive;
}

bool SearchManager::isCaseSensitive() const {
    return caseSensitive;
}

std::vector<int> SearchManager::getLastResults() const {
    return lastResults;
}

std::string SearchManager::getLastQuery() const {
    return lastQuery;
}

int SearchManager::getResultCount() const {
    return lastResults.size();
}

void SearchManager::notifyObservers() {
    for (auto observer : observers) {
        if (observer) {
            observer->onSearchResultsChanged(lastResults);
        }
    }
}
