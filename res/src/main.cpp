#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <vector>
#include <fstream>
#include "managers/AssetManager.h"
#include "managers/SceneManager.h"
#include "commands/Commands.h"
#include "python/PythonScript.h"

using sf::Event;
using sf::Mouse;
using std::cin;
using std::cout;
using std::string;
using std::rand;
using std::getline;
using std::istringstream;
using std::vector;
using std::ifstream;

//FUNCTION PROTOTYPES:
bool isCommand(const string& command, const vector<string>& args, const string& target);
void parseArgs(istringstream& parse, vector<string>& args);
void printAbout();

int main(int argc, char* argv[]) {

    //Load the python interpreter to keep it alive for as long as possible.
    scope = new py::scoped_interpreter{};

    //Testing...
    if (argc > 1 && string(argv[1]) == "-test") {

        sf::RenderWindow window(sf::VideoMode(1200, 800), "Media Compression by iCompression ~ TESTING MODE");
        window.setFramerateLimit(60); //Set the fps limit to 60 fps.
        //TODO: Make prefix work in final build.
        AssetManager("../"); //Set the prefix for the asset manager (CLion's default).
        AssetManager::loadAll();

        AssetManager::addFolder("../input");

        SceneManager viewManager(&window);
        SceneManager::addView("test", "../res/scenes/test.txt");
        SceneManager::addView("test2", "../res/scenes/test2.txt");

        SceneManager::changeView("test");

        while (window.isOpen()) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case (Event::Closed): {
                        window.close();
                        break;
                    }

                    case (Event::MouseButtonPressed): {
                        if (event.mouseButton.button == Mouse::Left) {
                            SceneManager::activateElement(Mouse::getPosition(window));
                        }
                        break;
                    }
                }
            }
            SceneManager::getView("test").getText("mxv")->setString(std::to_string(Mouse::getPosition(window).x));
            SceneManager::getView("test").getText("myv")->setString(std::to_string(Mouse::getPosition(window).y));
            window.clear(sf::Color::White);
            SceneManager::drawAll();
            window.display();
        }
        return 0;
    }

    //CLI Mode (Prioritize first for basic testing).
    else if (argc > 1 || string(argv[1]) == "-cli"){
        cout << "Media Compression by iCompression: CLI Mode" << endl;
        ifstream file;

        if (argc > 2 && string(argv[2]) == "-file"){
            file.open(argv[3]);
        }

        string line, command;
        do{
            command = "";
            if (argc <= 2) {
                cout << "SVDCompression> ";
                //1. Get the entire line of command.
                getline(cin, line);
            }
            else{
                if (file.is_open()){
                    getline(file, line);
                }
                else{
                    cerr << "file at " << argv[3] << " does not exist!";
                    exit(1);
                }
            }

            //2. Parse it into command parts.
            istringstream parse(line);
            vector<string> args;
            parse >> command;
            parseArgs(parse, args);

            //3. Run the appropriate command.
            if (isCommand(command, args, "add")){
                if (command == "help"){
                    getHelp(ADD);
                }
                else if (args.size() == 1){
                    AssetManager::addFile(args.at(0));
                }
            }

            else if (isCommand(command, args, "addf")){
                if (command == "help"){
                    getHelp(ADDF);
                }
                else if (args.size() == 1){
                    AssetManager::addFolder(args.at(0));
                }
            }

            else if (isCommand(command, args, "rm")){
                if (command == "help"){
                    getHelp(RM);
                }
                else if (args.size() == 1){
                    AssetManager::delFile(args.at(0));
                }
            }

            else if (isCommand(command, args, "rmf")){
                if (command == "help"){
                    getHelp(RMF);
                }
                else if (args.size() == 1){
                    AssetManager::delFolder(args.at(0));
                }
            }

            else if (isCommand(command, args, "ls")){
                if (command == "help"){
                    getHelp(LS);
                }
                else{
                    AssetManager::listFiles();
                }
            }

            else if (isCommand(command, args, "compress")){
                if (command == "help"){
                    getHelp(COMPRESS);
                }
                else if (args.size() == 1){
                    SVDAlgorithm(args.at(0), AssetManager::getFiles());
                }
            }

            else if (isCommand(command, args, "about")){
                if (command == "help"){
                    getHelp(ABOUT);
                }
                else{
                    printAbout();
                }
            }

            else if (isCommand(command, args, "help")){
                getHelp(HELP);
            }

            //Display all commands that are available along with their breakdown.
            else{
                if (command != "exit"){
                    listCommands();
                }
            }

        } while (command != "exit");


    }

    //The SFML Application.
    else{

    }

    //Close the python interpreter.
    delete scope;

}

bool isCommand(const string& command, const vector<string>& args, const string& target){
    return ((command == target) || (!args.empty() && args.at(0) == target));
}

void parseArgs(istringstream& parse, vector<string>& args){
    while(!parse.eof()){
        //Load the first part of the string.
        string i, temp;
        parse >> i;

        //If the first character is a quotation mark, keep loading until last character is a quotation mark.
        if (i.front() == '\"'){
            //Remove beginning quotation mark.
            i.erase(0, 1);

            while(i.back() != '\"' && !parse.eof()){
                parse >> temp;
                i += (" " + temp);
            }

            //Remove ending quotation mark.
            i.erase(i.end() - 1);
        }

        args.push_back(i);
    }
}

void printAbout(){
    ifstream file("README.md");

    if (file.is_open()){
        string line;
        while(getline(file, line)){
            cout << line << endl;
        }
    }
    else{
        cerr << "Unable to display information about the program. For information, please go to the github repository.";
    }
}