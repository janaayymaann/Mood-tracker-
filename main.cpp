#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
using namespace std;

//================ STRUCTS ================
struct Date
{
    int day, month, year;
};


struct MoodEntry
{
    Date date;
    string moodType;
    string note;
    int moodLevel;
    int userID;  /*not required as a member but i added it to know which
                  user each record belongs to*/
};


struct UserAccount
{
    int userID;
    string userName;
    string password;
};


struct MoodStatistics
{
    int totalEntries;
    int happyCount, sadCount, angryCount, stressedCount, calmCount;
    float happyAvg, sadAvg, angryAvg, stressedAvg, calmAvg;
};


//================ CONSTANTS ================
const int MOODRECORD = 100; //maximum number of mood records for all users 
const int USERACCOUNT = 10;
const int MONTHS = 12;

//================ ARRAYS ================
MoodStatistics statistics[MONTHS];
MoodEntry moods[MOODRECORD] =
{   // ===== 10 pre-stored records for 2 users =====
    {{2,1,2024}, "happy", "exam", 5, 1 },
    {{2,1,2024}, "sad", "bad day", 2, 2},
    {{3,1,2024}, "calm", "relaxed", 4, 1},
    {{4,1,2024}, "angry", "traffic", 1, 2},
    {{5,3,2024}, "happy", "friends", 5, 1},
    {{6,6,2024}, "stressed", "exam", 2, 2},
    {{7,5,2024}, "calm", "reading", 4, 1},
    {{8,12,2024}, "happy", "gym", 5, 2},
    {{9,10,2024}, "sad", "tired", 3, 1},
    {{10,12,2024}, "happy", "fun", 5, 2},
    // ===== User 3 =====
    {{11,1,2025}, "happy", "football match", 5, 3},
    {{12,1,2025}, "angry", "lost game", 2, 3},
    {{13,2,2025}, "calm", "watching TV", 4, 3},
    {{14,5,2025}, "stressed", "exam pressure", 3, 3},
    {{15,7,2025}, "happy", "friends outing", 5, 3},
    // ===== User 4 =====
    {{16,2,2025}, "sad", "long day", 2, 4},
    {{17,3,2025}, "happy", "family dinner", 5, 4},
    {{18,4,2025}, "stressed", "studying", 3, 4},
    {{19,8,2025}, "calm", "self care", 4, 4},
    {{20,8,2025}, "angry", "argument", 2, 4},
    // ===== User 5 =====
    {{21,1,2024}, "angry", "internet lag", 1, 5},
    {{22,7,2024}, "happy", "gaming win", 5, 5},
    {{23,8,2024}, "calm", "music", 4, 5},
    {{24,11,2024}, "stressed", "deadlines", 3, 5},
    {{25,12,2024}, "happy", "weekend", 5, 5},
    // ===== User 6 =====
    {{26,1,2026}, "stressed", "assignments", 3, 6},
    {{27,1,2026}, "happy", "shopping", 5, 6},
    {{28,2,2026}, "sad", "missed bus", 2, 6},
    {{1,4,2026}, "calm", "reading", 4, 6},
    {{2,6,2026}, "happy", "good news", 5, 6}
};
UserAccount users[USERACCOUNT] =
{  // ===== 2 pre-stored accounts =====
  { 1, "mohamed ahmed", "Mohamed123"},
  { 2, "mariam ali", "Marioma98" },
  // ===== remaining users  =====
  { 3, "omar mohamed", "Omar2006"},
  { 4, "Sarah Ayman", "Sarsoura" },
  { 5, "ali mahmoud", "Pubg123"},
  { 6, "noureen ahmed", "Noura222" },

};
//================ COUNTERS ================
int moodCount = 30;
int userCount = 6;

//================ INITIALIZATION ================
int currentUserId = -1;//before entering the program you don't have a user id 

//================ FUNCTION PROTOTYPES ================
bool login();
void signUp();
bool isValidPassword(string pass);
void logout();

void showMenu();

void addMood();
void updateMood();
void updateMoodWithDate(int day, int month, int year);
void deleteMood();


void viewAllMoods();

void searchMenu();
void searchByDate(int d, int m, int y);
void searchByMood(string type);

float calculateAverageForMoodTypeInMonth(int userId, string moodType, int month, int year);
void calculateMonthlyStatistics(int userId, int year);
void displayMonthlyStatistics(int userId, int year);


//================ HELPERS ================
string toLowerCase(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = tolower(text[i]);
    return text;
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year)
{
    if (month == 2) return isLeapYear(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

bool isValidDate(int day, int month, int year)
{
    if (year < 0 || month < 1 || month > 12) return false;
    int days = getDaysInMonth(month, year);
    return day >= 1 && day <= days;
}

bool isFutureDate(int day, int month, int year)
{
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);

    if (year > now.tm_year + 1900) return true;
    if (year == now.tm_year + 1900 && month > now.tm_mon + 1) return true;
    if (year == now.tm_year + 1900 && month == now.tm_mon + 1 && day > now.tm_mday) return true;
    return false;
}

//================ MAIN =================
int main()
{


    int choice;

    while (true)
    {
        if (currentUserId == -1)
        {
            cout << "\n=== Mood Tracker ===\n";
            cout << "1.Login\n2.Sign Up\n3.Exit\n";
            cout << "Choice: ";
            cin >> choice;

            if (choice == 1)
                login();

            else if (choice == 2) {
                signUp();
            }

            else if (choice == 3)
            {
                cout << "Goodbye!\n";

                return 0;
            }
            else
                cout << "Invalid choice.\n";
        }
        else
        {
            showMenu();
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                while (true)
                {
                    int subchoice;
                    cout << "\n1.Add Mood\n2.update Mood\n3.Delete Mood\n0.back\n";
                    cout << "Choice:\n";
                    cin >> subchoice;

                    if (subchoice == 0) break;
                    else if (subchoice == 1) addMood();
                    else if (subchoice == 2) updateMood();
                    else if (subchoice == 3) deleteMood();
                    else
                        cout << "invalid choice\n";
                }
                break;
            }

            case 2:
                viewAllMoods();
                break;

            case 3:
                searchMenu();
                break;

            case 4:
                int year;
                cout << "Enter year: ";
                cin >> year;
                calculateMonthlyStatistics(currentUserId, year);
                displayMonthlyStatistics(currentUserId, year);
                break;

            case 5:
                logout();
                break;

            case 6:

                cout << "Exiting...\n";
                return 0;
            }
        }
    }
}

//================ LOGIN =================
bool login()
{
    string name, pass;

    cin.ignore();

    cout << "Username: ";
    getline(cin, name);

    cout << "Password: ";
    cin >> pass;

    for (int i = 0; i < userCount; i++)
    {
        if (users[i].userName == name && users[i].password == pass)
        {
            currentUserId = users[i].userID;
            cout << "Login successful!\n";

            return true;
        }
    }

    cout << "[!] Incorrect username or password.\n";
    return false;
}

//================ SIGN UP =================
void signUp()
{
    // Added Limit Check For Usercount
    if (userCount >= USERACCOUNT)
    {
        cout << "Sorry ,no more users can register.\n";
        return;
    }

    UserAccount u;

    cin.ignore();

    cout << "New username: ";
    getline(cin, u.userName);

    do
    {
        cout << "Password (>=6 and one capital): ";
        cin >> u.password;
    } while (!isValidPassword(u.password));

    u.userID = userCount + 1;

    users[userCount] = u;
    userCount++;

    cout << "Account created.\n";
}

//================ PASSWORD CHECK =================
bool isValidPassword(string pass)
{
    if (pass.length() < 6) return false;

    bool capital = false;

    for (int i = 0; i < pass.length(); i++)
    {
        if (pass[i] >= 'A' && pass[i] <= 'Z')
            capital = true;
    }

    if (!capital)
    {
        cout << "[!] Must contain a capital letter.\n";
        return false;
    }

    return true;
}

//================ LOGOUT =================
void logout()
{
    currentUserId = -1;
    cout << "Logged out successfully.\n";
}

//================ MENU =================
void showMenu()
{
    cout << "\n===== Mood Tracker Menu =====\n";
    cout << "1.Manage Mood (add/update/delete)\n";
    cout << "2.View All Moods\n";
    cout << "3.Search Mood\n";
    cout << "4.View Analysis\n";
    cout << "5.Logout\n";
    cout << "6.Exit\n";
    cout << "Choice: ";
}

//================ ADD MOOD =================
void addMood()
{

 // Limit check for Moodcount
    if(moodCount>=MOODRECORD)
    {
        cout<<"Storage is full! Cannot add more moods.\n";
        return;
    }
    MoodEntry m;

    cout << "(0 to go back)\n";

    while (true)
    {
        cout << "Day: ";
        cin >> m.date.day;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid\n"; continue;
        }
        if (m.date.day == 0) return;

        cout << "Month: ";
        cin >> m.date.month;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid\n"; continue;
        }
        if (m.date.month == 0) return;

        cout << "Year: ";
        cin >> m.date.year;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid\n"; continue;
        }
        if (m.date.year == 0) return;


        if (!isValidDate(m.date.day, m.date.month, m.date.year))
        {
            cout << "Invalid date\n"; continue;
        }

        if (isFutureDate(m.date.day, m.date.month, m.date.year))
        {
            cout << "Future date not allowed\n"; continue;
        }

        //check duplicate date for same user
        for (int i = 0; i < moodCount; i++)
        {
            if (moods[i].userID == currentUserId &&
                moods[i].date.day == m.date.day &&
                moods[i].date.month == m.date.month &&
                moods[i].date.year == m.date.year)
            {
                cout << "[!] Record already exists for this date.\n";

                char c;
                while (true)
                {
                    cout << "Do you want to update it instead? (y/n): ";
                    cin >> c;

                    c = tolower(c);

                    if (c == 'y')
                    {

                        updateMoodWithDate(m.date.day, m.date.month, m.date.year);
                        return;
                    }
                    else if (c == 'n')
                    {

                        return;
                    }
                    else
                    {
                        cout << "Invalid choice.\n";
                    }
                }
            }
        }
        break;
    }

    string type;
    while (true)
    {
        cout << "Mood: ";
        cin >> type;

        if (type == "0")return;

        type = toLowerCase(type);

        if (type == "happy" || type == "sad" || type == "angry" || type == "calm" || type == "stressed")
        {
            m.moodType = type;
            break;
        }
        cout << "Invalid mood\n";
    }

    while (true)
    {
        cout << "Level 1-5: ";
        cin >> m.moodLevel;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid\n"; continue;
        }

        if (m.moodLevel >= 1 && m.moodLevel <= 5) break;

        cout << "Invalid level\n";
    }

    cin.ignore();
    cout << "Note: ";
    getline(cin, m.note);

    m.userID = currentUserId;

    moods[moodCount++] = m;

    cout << "Mood added successfully.\n";
}



//================ UPDATE MOOD =================

//========= HELPER (actual update logic) =========
void updateMoodWithDate(int day, int month, int year)
{
    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == currentUserId &&
            moods[i].date.day == day &&
            moods[i].date.month == month &&
            moods[i].date.year == year)
        {
            cout << "\nCurrent Mood Information:\n";
            cout << "Mood Type: " << moods[i].moodType << endl;
            cout << "Mood Level: " << moods[i].moodLevel << endl;
            cout << "Note: " << moods[i].note << endl;

            int ch;
            cout << "\n1.Edit Type\n2.Edit Level\n3.Edit Note\n4.Edit All\n";
            cin >> ch;
            cin.ignore();

            if (ch == 1)
            {
                string newType;
                while (true)
                {
                    cout << "Enter new mood type: ";
                    getline(cin, newType);

                    newType = toLowerCase(newType);

                    if (newType == "happy" || newType == "sad" ||
                        newType == "angry" || newType == "calm" ||
                        newType == "stressed")
                    {
                        moods[i].moodType = newType;
                        break;
                    }

                    cout << "Mood type not available.\n";
                }
            }

            else if (ch == 2)
            {
                while (true)
                {
                    cout << "Enter new mood level (1-5): ";
                    cin >> moods[i].moodLevel;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input.\n";
                        continue;
                    }

                    if (moods[i].moodLevel >= 1 && moods[i].moodLevel <= 5)
                        break;

                    cout << "Level must be 1 to 5.\n";
                }
            }

            else if (ch == 3)
            {
                cin.ignore();
                cout << "Enter new note: ";
                getline(cin, moods[i].note);
            }

            else if (ch == 4)
            {
                string newType;

                while (true)
                {
                    cout << "Enter new mood type: ";
                    getline(cin, newType);

                    newType = toLowerCase(newType);

                    if (newType == "happy" || newType == "sad" ||
                        newType == "angry" || newType == "calm" ||
                        newType == "stressed")
                    {
                        moods[i].moodType = newType;
                        break;
                    }

                    cout << "Mood type not available.\n";
                }

                while (true)
                {
                    cout << "Enter new mood level (1-5): ";
                    cin >> moods[i].moodLevel;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input.\n";
                        continue;
                    }

                    if (moods[i].moodLevel >= 1 && moods[i].moodLevel <= 5)
                        break;

                    cout << "Level must be 1 to 5.\n";
                }

                cin.ignore();
                cout << "Enter new note: ";
                getline(cin, moods[i].note);
            }

            else
            {
                cout << "Invalid choice.\n";
                return;
            }

            cout << "Mood updated successfully.\n";
            return;
        }
    }

    cout << "Mood entry not found.\n";
}

//========= MAIN UPDATE FUNCTION =========
void updateMood()
{
    int day, month, year;

    cout << "Enter date of mood to update:\n";
    cout << "(enter 0 anytime to go back)\n";

    while (true)
    {
        cout << "Day: ";
        cin >> day;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (day == 0) return;

        cout << "Month: ";
        cin >> month;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (month == 0) return;

        cout << "Year: ";
        cin >> year;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (year == 0) return;

        if (!isValidDate(day, month, year))
        {
            cout << "Invalid date\n";
            continue;
        }

        if (isFutureDate(day, month, year))
        {
            cout << "Future date not allowed\n";
            continue;
        }

        break;
    }

    // call main logic
    updateMoodWithDate(day, month, year);
}
//================ DELETE =================
void deleteMood()
{

    int day, month, year;

    cout << "Enter date of mood to delete:\n";
    cout << "(enter 0 anytime to go back)\n";

    /*loop to validate date input (repeats until valid*/
    while (true)
    {
        //DAY LOOP
        while (true)
        {
            cout << "Day: ";
            cin >> day;

            if (cin.fail()) //check if the input not an integer
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again.\n";
                continue;
            }

            if (day == 0) return;
            break;
        }

        // MONTH LOOP
        while (true)
        {
            cout << "Month: ";
            cin >> month;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again.\n";
                continue;
            }

            if (month == 0) return;
            break;
        }

        // YEAR LOOP
        while (true)
        {
            cout << "Year: ";
            cin >> year;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Try again.\n";
                continue;
            }

            if (year == 0) return;
            break;
        }
        if (isValidDate(day, month, year) == false)
        {
            cout << "Invalid date\n"; continue;
        }

        if (isFutureDate(day, month, year))
        {
            cout << "Future date not allowed\n"; continue;
        }

        break;
    }

    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == currentUserId &&
            moods[i].date.day == day &&
            moods[i].date.month == month &&
            moods[i].date.year == year)
        {
            for (int j = i; j < moodCount - 1; j++) //shift element left to fill deleted position
                moods[j] = moods[j + 1];

            moodCount--;//remove duplicate after shifting

            cout << "Mood deleted.\n";
            return;
        }
    }

    cout << "Mood not found.\n";
}

//================ VIEW ALL MOODS =================
void viewAllMoods()
{
    cout << "==========================================================\n";
    cout << "                  ALL MOOD RECORDS LIST                    \n";
    cout << "==========================================================\n";

    if (moodCount == 0)
    {
        cout << "No moods recorded.\n";
        return;
    }
    int cmoodcount = 0; //current user mood count
    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == currentUserId)
        {
            cmoodcount++;
            cout << "\nRecord #" << (cmoodcount) << ":";
            cout << "\nDate: " << moods[i].date.day << "/" << moods[i].date.month << "/" << moods[i].date.year;
            cout << "\nMood: " << moods[i].moodType;
            cout << "\nLevel: " << moods[i].moodLevel << "/5\n";
            cout << "Note: " << moods[i].note << "\n";
            cout << "----------------------------------------------------------\n";
        }
    }
}

//================ SEARCH =================
void searchMenu()
{
    int choice;
    while (true)
    {
        cout << "---- SEARCH MENU ----\n";
        cout << "1. Search by date\n";
        cout << "2. Search by mood\n";
        cout << "0. back\n";
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 0) break;
        switch (choice)
        {
        case 1:
        {
            int day, month, year;

            cout << "Enter date of mood to search:\n";
            cout << "(enter 0 anytime to go back)\n";
            int flag = 0;
            // DAY
            while (true)
            {
                while (true)
                {
                    cout << "Day: ";
                    cin >> day;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Try again.\n";
                        continue;
                    }

                    if (day == 0) flag = 1;
                    break;
                }
                if (flag == 1)break;

                // MONTH
                while (true)
                {
                    cout << "Month: ";
                    cin >> month;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Try again.\n";
                        continue;
                    }

                    if (month == 0) flag = 1;
                    break;
                }
                if (flag == 1)break;

                // YEAR
                while (true)
                {
                    cout << "Year: ";
                    cin >> year;

                    if (cin.fail())
                    {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Try again.\n";
                        continue;
                    }

                    if (year == 0) flag = 1;
                    break;
                }
                if (flag == 1)break;
                if (!isValidDate(day, month, year))
                {
                    cout << "Invalid date\n";
                    continue;
                }

                if (isFutureDate(day, month, year))
                {
                    cout << "Future date not allowed\n"; continue;
                }
                break;
            }
            searchByDate(day, month, year);
            break;

        }
        break;


        case 2:
        {
            string type;
            cout << "(enter 0 anytime to go back )\n";
            cout << "Enter mood type: ";
            cin >> type;
            if (type == "0")continue;
            searchByMood(type);
            break;
        }

        default:
            cout << "Invalid choice.\n";
        }
    }
}

void searchByDate(int day, int month, int year)
{
    bool found = false;

    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == currentUserId &&
            moods[i].date.day == day &&
            moods[i].date.month == month &&
            moods[i].date.year == year)
        {
            cout << "Date: " << moods[i].date.day << "/"
                << moods[i].date.month << "/"
                << moods[i].date.year << endl;

            cout << "Mood: " << moods[i].moodType << endl;
            cout << "Note: " << moods[i].note << endl;
            cout << "Level: " << moods[i].moodLevel << endl;
            cout << "User ID: " << moods[i].userID << endl;

            cout << "-------------------\n";

            found = true;
        }
    }

    if (!found)
        cout << "No entries found for this date.\n";
}

void searchByMood(string type)
{
    type = toLowerCase(type);

    if (!(type == "happy" || type == "sad" ||
        type == "angry" || type == "calm" ||
        type == "stressed"))
    {
        cout << "Mood type not available.\n";
        return;
    }

    bool found = false;

    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == currentUserId &&
            moods[i].moodType == type)
        {
            cout << "Date: " << moods[i].date.day << "/"
                << moods[i].date.month << "/"
                << moods[i].date.year << endl;

            cout << "Mood: " << moods[i].moodType << endl;
            cout << "Level: " << moods[i].moodLevel << endl;
            cout << "Note: " << moods[i].note << endl;
            cout << "-------------------\n";

            found = true;
        }
    }

    if (!found)
        cout << "No entries found for this mood.\n";
}

//===============AVERAGE CALCULATION================
     // Calculate Average for each Mood Type

float calculateAverageForMoodTypeInMonth(int userId, string moodType, int month, int year)
{
    int sum = 0, count = 0;

    moodType = toLowerCase(moodType);

    for (int i = 0; i < moodCount; i++)
    {
        if (moods[i].userID == userId &&
            moods[i].moodType == moodType &&
            moods[i].date.month == month &&
            moods[i].date.year == year)   // bonus
        {
            sum += moods[i].moodLevel;
            count++;
        }
    }

    if (count == 0) return 0;
    return (float)sum / count;
}

void calculateMonthlyStatistics(int userId, int year)
{
    for (int m = 0; m < 12; m++) //m for month
    {
        statistics[m].totalEntries = 0;

        statistics[m].happyCount = 0;
        statistics[m].sadCount = 0;
        statistics[m].angryCount = 0;
        statistics[m].stressedCount = 0;
        statistics[m].calmCount = 0;

        for (int i = 0; i < moodCount; i++)
        {
            if (moods[i].userID == userId &&
                moods[i].date.month == m + 1 &&
                moods[i].date.year == year)   
            {
                statistics[m].totalEntries++;

                string type = moods[i].moodType;

                if (type == "happy") statistics[m].happyCount++;
                else if (type == "sad") statistics[m].sadCount++;
                else if (type == "angry") statistics[m].angryCount++;
                else if (type == "stressed") statistics[m].stressedCount++;
                else if (type == "calm") statistics[m].calmCount++;
            }
        }

        // averages (per month + year)
        statistics[m].happyAvg = calculateAverageForMoodTypeInMonth(userId, "happy", m + 1, year);
        statistics[m].sadAvg = calculateAverageForMoodTypeInMonth(userId, "sad", m + 1, year);
        statistics[m].angryAvg = calculateAverageForMoodTypeInMonth(userId, "angry", m + 1, year);
        statistics[m].stressedAvg = calculateAverageForMoodTypeInMonth(userId, "stressed", m + 1, year);
        statistics[m].calmAvg = calculateAverageForMoodTypeInMonth(userId, "calm", m + 1, year);
    }
}
void displayMonthlyStatistics(int userId, int year)
{
    cout << "\n===== Monthly Statistics =====\n";

    for (int m = 0; m < 12; m++)
    {
        cout << "\nMonth " << m + 1 << ":\n";
        cout << "Total: " << statistics[m].totalEntries << "\n";
        cout << "Happy Avg: " << statistics[m].happyAvg << "\n";
        cout << "Sad Avg: " << statistics[m].sadAvg << "\n";
        cout << "Angry Avg: " << statistics[m].angryAvg << "\n";
        cout << "Stressed Avg: " << statistics[m].stressedAvg << "\n";
        cout << "Calm Avg: " << statistics[m].calmAvg << "\n";
    }
}
