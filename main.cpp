#include <iostream>
#include <map>
#include <fstream>
#include "csvstream.hpp"
#include <set>
#include <cmath>

void printError() {
    std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
}

class Classifier {
    int _numPosts;
    int _numUniqueWords;
    std::map<std::string, int> _postsWithWord;
    std::map<std::string, int> _postsWithLabel;
    std::map<std::pair<std::string, std::string>, int> _postsWithLabelWord;
    bool _debug;

    /// @brief Returns the number of unique words in a string
    /// @param str The string to parse
    /// @return A set containing all of the unique words in the given string
    static std::set<std::string> uniqueWords(const std::string &str) {
        std::istringstream source(str);
        std::set<std::string> words;
        std::string word;
        while (source >> word)
            words.insert(word);
        return words;
    }

    double logPrior(int numPostsWithC) {
        double res = numPostsWithC / (_numPosts * 1.0);
        return log(res);
    }

    double logLikelihood(
        int numPostsCWithW, 
        int numPostsC,
        int numPostsW
    ) {
        if(numPostsCWithW == 0 && numPostsW == 0)
            return log(1.0 / _numPosts);
        else if(numPostsCWithW == 0)
            return log((1.0 * numPostsW) / _numPosts);

        return log(numPostsCWithW / (1.0 * numPostsC));
    } 

    double logProbability(
        const std::set<std::string>& uniqueWords,
        const std::string& label
    ) {
        double total = 0;
        for(const auto& word : uniqueWords) {
            int CW  = _postsWithLabelWord[std::make_pair(label, word)];
            int l = _postsWithLabel[label];
            int w = _postsWithWord[word];
            total += logLikelihood(CW, l, w);
        }
        return logPrior(_postsWithLabel[label]) + total; 
    }

public:
    Classifier(bool debug) : _debug(debug) {
        _numPosts = 0;
        _numUniqueWords = 0;
    }

    void train(csvstream& csv) {
        std::map<std::string, std::string> inMap;
        
        if(_debug)
            std::cout << "training data:" << std::endl;
        
        while(csv >> inMap) {
            _postsWithLabel[inMap["tag"]] += 1;
            auto words = uniqueWords(inMap["content"]);

            for(auto s : words) {
                _postsWithWord[s] += 1;
                _postsWithLabelWord[std::make_pair(inMap["tag"], s)] += 1;
            }

            _numPosts += 1;

            if(_debug)
                std::cout << "  label = " << inMap["tag"] 
                    << ", content = " << inMap["content"]
                    << std::endl; 
        }

        _numUniqueWords = _postsWithWord.size();

        // Log info
        std::cout << "trained on " << _numPosts << " examples" << std::endl;
        if(_debug)
            std::cout << "vocabulary size = " << _numUniqueWords << std::endl;
        std::cout << std::endl;
    }

    void predict(csvstream& testCsv) {
        if(_debug) {
            std::cout << "classes:" << std::endl;
            for(std::pair<std::string, int> e : _postsWithLabel) {
                std::cout << "  " << e.first << ", " << e.second << " examples, "
                    << "log-prior = " << logPrior(e.second) << std::endl;
            }

            std::cout << "classifier parameters:" << std::endl;
            for(const auto& e : _postsWithLabelWord) {
                double res = logLikelihood(
                    e.second,
                    _postsWithLabel.at(e.first.first),
                    _postsWithWord.at(e.first.second)
                );
                std::cout << "  " << e.first.first << ":" << e.first.second 
                    << ", count = " << e.second << ", "
                    << "log-likelihood = " << res << std::endl;
            }
            
            std::cout << std::endl;
        }

        // Actual classifier
        std::cout << "test data:" << std::endl;

        std::set<std::string> labels;
        for(const auto& e : _postsWithLabel)
            labels.insert(e.first);

        std::map<std::string, std::string> inMap;
        int numPredictedCorrect = 0;
        int totalPredicted = 0;
        while(testCsv >> inMap) {
            auto words = uniqueWords(inMap["content"]);

            double highestProbability;
            std::string highestPrediction;
            for(const auto& label : labels) {
                double probability = logProbability(words, label);
                if(highestPrediction == "" || probability > highestProbability) {
                    highestPrediction = label;
                    highestProbability = probability;
                }
            }

            std::cout << "  correct = " << inMap["tag"] 
                << ", predicted = " << highestPrediction
                << ", log-probability score = " << highestProbability << std::endl
                << "  content = " << inMap["content"] << std::endl << std::endl;

            if(inMap["tag"] == highestPrediction)
                numPredictedCorrect++;
            totalPredicted++;
        }

        std::cout 
            << "performance: " << numPredictedCorrect << " / " << totalPredicted
            << " posts predicted correctly" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::cout.precision(3);

    if(argc < 3 || argc > 4) {
        printError();
        return 1;
    }

    std::string trainFileName = argv[1];
    std::string testFileName = argv[2];

    try {
        csvstream trainCsv{trainFileName};
        csvstream testCsv{testFileName};

        if(argc == 4 && strcmp(argv[3], "--debug") != 0) {
            printError();
            return 4;
        }

        const bool debug = argc == 4;

        Classifier classifier(debug);

        classifier.train(trainCsv);
        classifier.predict(testCsv);
    } catch(const csvstream_exception& e) {
        std::cout << e.what() << std::endl;
        return 2;
    }

    return 0;
}