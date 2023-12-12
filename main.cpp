#include <iostream>
#include <map>
#include <fstream>
#include "csvstream.hpp"
#include <set>
#include <cmath>

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

    /// @brief Calculates how common a post with the given label C is
    /// @param numPostsWithC The label to check
    /// @return A value indicating the probability where 1 is most common
    double logPrior(int numPostsWithC) {
        double res = numPostsWithC / (_numPosts * 1.0);
        return log(res);
    }

    /// @brief Calculates the likelihood of a word W given label C. 
    ///        This is a measurement of how likely it to see w in a post labeled C
    /// @param numPostsCWithW The number of posts with label C that contain W
    /// @param numPostsC The number of posts with label C
    /// @param numPostsW The number of posts with word W.
    /// @return A value indicating how likely it is to obserb w in a post labeled C.
    ///         1 means most likely.
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

    /// @brief Determines the log probability for a set of unique words given a label
    /// @param uniqueWords A set containing unique words
    /// @param label The label
    /// @return A value indicating how probable the label is the correct label
    ///         for a given set of words
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
    /// @brief Initiaizes the classifier with a debug mode. Should be called before
    ///        'predict'
    /// @param debug If true, debug information will be output for the classifier
    Classifier(bool debug) : _debug(debug) {
        _numPosts = 0;
        _numUniqueWords = 0;
    }

    /// @brief Trains the classifier on a set of data
    /// @param csv The CSV containing the training data
    void train(csvstream& csv) {        
        if(_debug)
            std::cout << "training data:" << std::endl;
        
        // Read in each row of the training data
        std::map<std::string, std::string> inMap;
        while(csv >> inMap) {
            _postsWithLabel[inMap["tag"]] += 1;
            auto words = uniqueWords(inMap["content"]);

            // Determine the number of times each word occurs
            // and how many times they occur for a given label
            for(auto s : words) {
                _postsWithWord[s] += 1;
                _postsWithLabelWord[std::make_pair(inMap["tag"], s)] += 1;
            }

            _numPosts += 1;

            if(_debug) {
                std::cout << "  label = " << inMap["tag"] 
                    << ", content = " << inMap["content"]
                    << std::endl; 
            }
        }

        _numUniqueWords = _postsWithWord.size();

        // Log info
        std::cout << "trained on " << _numPosts << " examples" << std::endl;
        if(_debug)
            std::cout << "vocabulary size = " << _numUniqueWords << std::endl;
        std::cout << std::endl;
    }

    /// @brief For a CSV, attempts to predict the label for each post 
    /// @param testCsv The CSV containing the data to predict
    void predict(csvstream& testCsv) {
        // Log debug information
        if(_debug) {
            // Prints all the labels
            std::cout << "classes:" << std::endl;
            for(std::pair<std::string, int> e : _postsWithLabel) {
                std::cout << "  " << e.first << ", " << e.second << " examples, "
                    << "log-prior = " << logPrior(e.second) << std::endl;
            }

            // Prints labels matched with words
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

        // Place the different labels into a set
        std::set<std::string> labels;
        for(const auto& e : _postsWithLabel)
            labels.insert(e.first);

        // Read in the input data
        std::map<std::string, std::string> inMap;
        int numPredictedCorrect = 0;
        int totalPredicted = 0;
        while(testCsv >> inMap) {
            auto words = uniqueWords(inMap["content"]);

            // Determine the label with the highest probability
            double highestProbability;
            std::string highestPrediction;
            for(const auto& label : labels) {
                double probability = logProbability(words, label);
                if(highestPrediction == "" || probability > highestProbability) {
                    highestPrediction = label;
                    highestProbability = probability;
                }
            }

            // Print prediction info
            std::cout << "  correct = " << inMap["tag"] 
                << ", predicted = " << highestPrediction
                << ", log-probability score = " << highestProbability << std::endl
                << "  content = " << inMap["content"] << std::endl << std::endl;

            // Update totals
            if(inMap["tag"] == highestPrediction)
                numPredictedCorrect++;
            totalPredicted++;
        }

        // Print performance information
        std::cout 
            << "performance: " << numPredictedCorrect << " / " << totalPredicted
            << " posts predicted correctly" << std::endl;
    }
};

/// @brief Logs an error message for command line argument errors
void printError() {
    std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout.precision(3);

    if(argc < 3 || argc > 4) {
        printError();
        return 1;
    }

    if(argc == 4) {
        std::string debugStr = argv[3];
        if(debugStr != "--debug") {
            printError();
            return 4;
        }
    }

    std::string trainFileName = argv[1];
    std::string testFileName = argv[2];

    try {
        csvstream trainCsv{trainFileName};
        csvstream testCsv{testFileName};

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