 #include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <string>
#include <windows.h>
#include <iomanip>
#include <cstring>
#include <map>
#include <bitset> // Добавлено

struct Record
{
    char user_fullname[30];
    unsigned short int deposit_summ;
    char deposit_date[10];
    char advocate_fullname[22];
};

struct tree
{
    Record *record;
    tree *L = nullptr;
    tree *R = nullptr;
    int balance = 0;
};

struct date
{
    int year;
    int month;
    int day;
};

// Вывод заголовка таблицы
void printTableHeader()
{
    std::cout << "+-----+--------------------------------+-----------------+-------+-------+------------+\n";
    std::cout << "| " << std::setw(3) << "№"
              << " | " << std::setw(30) << "ФИО вкладчика"
              << " | " << std::setw(10) << "Сумма вклада"
              << " | " << std::setw(10) << "Дата вклада"
              << " | " << std::setw(22) << "ФИО адвоката"
                                           " |\n";
    std::cout << "+-----+--------------------------------+-----------------+-------+-------+------------+\n";
}

// Вывод одной записи
void printRecord(const Record *rec, int index)
{
    std::cout << "| " << std::setw(3) << (index + 1) << " | ";

    system("chcp 866 > nul");
    std::cout << std::setw(30) << rec->user_fullname;

    std::cout << " | ";

    std::cout << std::setw(5) << rec->deposit_summ;

    std::cout << " | ";

    std::cout << std::setw(5) << rec->deposit_date;

    std::cout << " | ";

    std::cout << std::setw(10) << rec->advocate_fullname;
    system("chcp 65001 > nul");

    std::cout << " |\n";
}

void printRecord(const Record *rec)
{
    std::cout << "|      | ";

    system("chcp 866 > nul");
    std::cout << std::setw(30) << rec->user_fullname;

    std::cout << " | ";

    std::cout << std::setw(5) << rec->deposit_summ;

    std::cout << " | ";

    std::cout << std::setw(5) << rec->deposit_date;

    std::cout << " | ";

    std::cout << std::setw(10) << rec->advocate_fullname;
    system("chcp 65001 > nul");

    std::cout << " |\n";
}

// Вывод страницы записей
void printPage(int n, Record *indexArray[], int totalRecords)
{
    system("CLS");
    printTableHeader(); // Печатаем заголовок таблицы
    int start = n;
    int end = (n + 20 > totalRecords) ? totalRecords : n + 20;

    for (int i = start; i < end; ++i)
    {
        printRecord(indexArray[i], i);
    }

    // Добавляем нижнюю границу таблицы
    std::cout << "+-----+--------------------------------+-----------------+-------+-------+------------+\n";
}

void printPageAVL(int n, const std::vector<Record> &records, int totalRecords)
{
    system("CLS");
    printTableHeader();
    int start = n;
    int end = (n + 20 < totalRecords) ? n + 20 : totalRecords;

    for (int i = start; i < end; ++i)
    {
        printRecord(&records[i], i);
    }

    // Добавляем нижнюю границу таблицы
    std::cout << "+-----+--------------------------------+-----------------+-------+-------+------------+\n";
}

date stringToDate(Record *rec)
{ // Преобразование строки в дату
    date result;
    char buf[10];
    memcpy(buf, rec->deposit_date, 2);
    result.day = std::stoi(buf);
    memcpy(buf, &rec->deposit_date[3], 2);
    result.month = std::stoi(buf);
    memcpy(buf, &rec->deposit_date[6], 2);
    result.year = std::stoi(buf);

    return result;
}

int compDate(Record *rec1, Record *rec2)
{
    date d1 = stringToDate(rec1);
    date d2 = stringToDate(rec2);

    // Сравниваем годы
    if (d1.year != d2.year)
    {
        return d1.year - d2.year;
    }
    // Сравниваем месяцы
    if (d1.month != d2.month)
    {
        return d1.month - d2.month;
    }
    // Сравниваем дни
    return d1.day - d2.day; // Вернет 0, если равны, -1 если меньше, 1 если больше
}

// Добавление элемента в кучу
void AddElementToHeap1(Record *arr[], int L, int R)
{
    Record *x = arr[L - 1];

    int i = L;
    while (true)
    {
        int j = 2 * i;
        if (j > R)
            break;
        if (j < R)
        {
            int cmpK = arr[j]->deposit_summ > arr[j - 1]->deposit_summ;
            if (cmpK > 0 || (cmpK == 0 && (arr[j]->deposit_summ > arr[j - 1]->deposit_summ)) ||
                (cmpK == 0 && arr[j]->deposit_summ == arr[j - 1]->deposit_summ && compDate(arr[j], arr[j - 1]) > 0))
                j++;
        }

        int cmpK = x->deposit_summ > arr[j - 1]->deposit_summ;
        if (cmpK > 0 || (cmpK == 0 && x->deposit_summ > arr[j - 1]->deposit_summ) ||
            (cmpK == 0 && x->deposit_summ == arr[j - 1]->deposit_summ && compDate(x, arr[j - 1]) > 0))
            break;

        arr[i - 1] = arr[j - 1];
        i = j;
    }
    arr[i - 1] = x;
}

// Сортировка массива
void HeapSort(Record *arr[], int size)
{
    int L = size / 2;
    while (L > 0)
    {
        AddElementToHeap1(arr, L, size);
        L--;
    }
    int R = size;
    while (R > 1)
    {
        Record *temp = arr[0];
        arr[0] = arr[R - 1];
        arr[R - 1] = temp;

        R--;
        AddElementToHeap1(arr, 1, R);
    }

    std::cout << "База данных отсортирована по ключу, вкладу и его дате.\n";
}

// Функция для сравнения элементов по сумме вклада
bool compareByDepositAmount(const Record &a, const Record &b)
{
    return a.deposit_summ < b.deposit_summ; // Сравнение по возрастанию суммы
}

int BinarySearch(Record **records, int totalRecords, unsigned short int deposit_summ)
{
    int L = 0, R = totalRecords - 1;
    int flag = 0;
    int m;

    while (L < R)
    {
        m = (L + R) / 2;
        if (records[m]->deposit_summ < deposit_summ)
            L = m + 1;
        else
            R = m;
    }

    if (records[R]->deposit_summ == deposit_summ)
        flag = 1;

    if (flag == 1)
        return R;
    else
        return -1;
}

// Функция для выполнения двоичного поиска в дереве
int BinarySearchAVL(Record *records, int totalRecords, unsigned short int deposit_summ)
{
    int L = 0, R = totalRecords - 1;

    while (L <= R)
    {
        int m = L + (R - L) / 2; // Избегание переполнения

        if (records[m].deposit_summ < deposit_summ)
        {
            L = m + 1;
        }
        else if (records[m].deposit_summ > deposit_summ)
        {
            R = m - 1;
        }
        else
        {
            return m; // Вернуть индекс, если найдено
        }
    }
    return -1; // Если не найдено
}

int findRecordsAmount(Record **records, int totalRecords, int depositAmount, int amount)
{
    amount = 0;
    for (int i = 0; i < totalRecords; i++)
    {
        if (records[i]->deposit_summ == depositAmount)
        {
            amount++;
        }
    }
    return amount;
}

Record **requiredRecords(Record **indexArray, Record **indexArrayKey, int index, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        indexArrayKey[i] = indexArray[index + i];
    }
    return indexArrayKey;
}

void printRequiredRecords(Record **records, int amount)
{
    printTableHeader();
    for (int i = 0; i < amount; i++)
    {
        std::cout << "| " << std::setw(3) << (i + 1) << " | ";

        system("chcp 866 > nul"); // Для правильного отображения русских символов
        std::cout << std::setw(30) << records[i]->user_fullname;
        system("chcp 65001 > nul");

        std::cout << " | ";

        system("chcp 866 > nul");
        std::cout << std::setw(5) << records[i]->deposit_summ;
        system("chcp 65001 > nul");

        std::cout << " | ";

        system("chcp 866 > nul");
        std::cout << std::setw(5) << records[i]->deposit_date;
        system("chcp 65001 > nul");

        std::cout << " | ";

        system("chcp 866 > nul");
        std::cout << std::setw(10) << records[i]->advocate_fullname;
        system("chcp 65001 > nul");

        std::cout << " |\n";
    }
}

// Функции для поворотов АВЛ-дерева
void rotateRR(tree *&p)
{
    tree *q = p->R;
    p->balance = 0;
    q->balance = 0;
    p->R = q->L;
    q->L = p;
    p = q;
}

void rotateLR(tree *&p)
{
    tree *q = p->L;
    tree *r = q->R;
    if (r->balance < 0)
        p->balance = 1;
    else
        p->balance = 0;
    if (r->balance > 0)
        q->balance = -1;
    else
        q->balance = 0;
    r->balance = 0;
    q->R = r->L;
    p->L = r->R;
    r->L = q;
    r->R = p;
    p = r;
}

void rotateRL(tree *&p)
{
    tree *q = p->R;
    tree *r = q->L;
    if (r->balance > 0)
        p->balance = -1;
    else
        p->balance = 0;
    if (r->balance < 0)
        q->balance = 1;
    else
        q->balance = 0;
    r->balance = 0;
    q->L = r->R;
    p->R = r->L;
    r->R = q;
    r->L = p;
    p = r;
}

void rotateLL(tree *&p)
{
    tree *q = p->L;
    p->balance = 0;
    q->balance = 0;
    p->L = q->R;
    q->R = p;
    p = q;
}

bool addAVL(tree *&p, Record *&record, bool &rost)
{
    if (p == nullptr)
    {
        p = new tree;
        p->record = record;
        rost = true;
    }
    else if (strcmp(record->advocate_fullname, p->record->advocate_fullname) < 0)
    {
        if (addAVL(p->L, record, rost))
        {
            if (rost)
            {
                if (p->balance > 0)
                {
                    p->balance = 0;
                    rost = false;
                }
                else if (p->balance == 0)
                {
                    p->balance = -1;
                    rost = true;
                }
                else if (p->L->balance < 0)
                {
                    rotateLL(p);
                    rost = false;
                }
                else
                {
                    rotateLR(p);
                    rost = false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    else if (strcmp(record->advocate_fullname, p->record->advocate_fullname) >= 0)
    {
        if (addAVL(p->R, record, rost))
        {
            if (rost)
            {
                if (p->balance < 0)
                {
                    p->balance = 0;
                    rost = false;
                }
                else if (p->balance == 0)
                {
                    p->balance = 1;
                    rost = true;
                }
                else if (p->R->balance > 0)
                {
                    rotateRR(p);
                    rost = false;
                }
                else
                {
                    rotateRL(p);
                    rost = false;
                }
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

tree *buildAVLTree(Record **indexArrayKey, int amount)
{
    tree *root = nullptr;
    bool rost = false;
    for (int i = 0; i < amount; i++)
    {
        addAVL(root, indexArrayKey[i], rost);
    }
    return root;
}

void inOrderTraversal(tree *p)
{
    if (p != nullptr)
    {
        inOrderTraversal(p->L);
        printRecord(p->record);
        inOrderTraversal(p->R);
    }
}

int SearchInTree(tree *p, int c, const char *advocatKey)
{
    if (p != nullptr)
    {
        if (strncmp(advocatKey, p->record->advocate_fullname, 3) > 0)
        {
            c = SearchInTree(p->R, c, advocatKey);
        }
        if (strncmp(advocatKey, p->record->advocate_fullname, 3) < 0)
        {
            c = SearchInTree(p->L, c, advocatKey);
        }
        if (strncmp(advocatKey, p->record->advocate_fullname, 3) == 0)
        {
            c = SearchInTree(p->L, c, advocatKey);
            printRecord(p->record);
            c++;
            c = SearchInTree(p->R, c, advocatKey);
        }
    }
    return c;
}

// Структура узла дерева Хаффмана
struct HuffmanNode
{
    char ch;
    int freq;
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

// Компаратор для приоритетной очереди
struct CompareNode
{
    bool operator()(HuffmanNode *const &a, HuffmanNode *const &b) const
    {
        return a->freq > b->freq;
    }
};

// Построение дерева Хаффмана
HuffmanNode *buildHuffmanTree(const std::unordered_map<char, int> &frequencies)
{
    std::priority_queue<HuffmanNode *, std::vector<HuffmanNode *>, CompareNode> pq;

    for (const auto &pair : frequencies)
    {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1)
    {
        HuffmanNode *leftNode = pq.top();
        pq.pop();
        HuffmanNode *rightNode = pq.top();
        pq.pop();

        HuffmanNode *parentNode = new HuffmanNode('\0', leftNode->freq + rightNode->freq);
        parentNode->left = leftNode;
        parentNode->right = rightNode;

        pq.push(parentNode);
    }

    return pq.top();
}

// Рекурсивная функция для получения кодов символов
void getHuffmanCodes(HuffmanNode *root, const std::string &code, std::unordered_map<char, std::string> &codeMap)
{
    if (!root)
        return;

    // if ((int)root->ch != 0)
    // {
    codeMap[(int)root->ch] = code;
    // }

    getHuffmanCodes(root->left, code + "0", codeMap);
    getHuffmanCodes(root->right, code + "1", codeMap);
}

// Освобождение памяти дерева Хаффмана
void freeHuffmanTree(HuffmanNode *root)
{
    if (!root)
        return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    delete root;
}

void encodeMenuOption()
{
    // system("CLS");
    // system("chcp 866 > nul");
    std::string filename = "testBase3.dat";

    // Read characters and count frequencies
    std::unordered_map<char, int> frequencies;
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return;
    }

    char ch;
    int totalSymbols = 0;

    // sanechkeno
    while (!file.read((char *)&ch, sizeof(ch)).eof())
    {
        totalSymbols++;
        if (int(ch) < 0)
        {
            frequencies[int(ch) + 256]++;
        }
        else
        {
            frequencies[int(ch)]++;
        }
    }

    // while (file.get(ch)) {
    //     if((int)ch < 0) {
    //         frequencies[(int)ch + 256]++;
    //     } else{
    //         frequencies[ch]++;
    //     }
    //     totalSymbols++;
    // }
    file.close();

    // Build Huffman tree
    HuffmanNode *root = buildHuffmanTree(frequencies);

    // Get Huffman codes
    std::unordered_map<char, std::string> codeMap;
    getHuffmanCodes(root, "", codeMap);

    // Store frequencies and codes in a vector for sorting
    std::vector<std::pair<char, int>> freqVector(frequencies.begin(), frequencies.end());
    sort(freqVector.begin(), freqVector.end(), [&](const std::pair<char, int> &a, const std::pair<char, int> &b)
         { return static_cast<double>(a.second) / totalSymbols > static_cast<double>(b.second) / totalSymbols; });

    // system("chcp 65001 > nul");

    // Print sorted frequency and probability table
    std::cout << "+--------+------------+---------------------+---------------------+---------------------+\n";
    std::cout << "| Символ | Частота    | Вероятность         | Длина кодового слова | Кодовое слово       |\n";
    std::cout << "+--------+------------+---------------------+---------------------+---------------------+\n";

    system("chcp 866 > nul");
    for (const auto &pair : freqVector)
    {
        char symbol = pair.first;
        int count = pair.second;
        double probability = static_cast<double>(count) / totalSymbols;
        int length = codeMap[symbol].length();
        std::string code = codeMap[symbol];

        // (isprint(symbol) ? string(1, symbol) : " ")
        std::cout << "| " << std::setw(6) << symbol
                  << " | " << std::setw(10) << count
                  << " | " << std::setw(19) << probability
                  << " | " << std::setw(19) << length
                  << " | " << std::setw(19) << code << " |\n";
    }

    std::cout << "+--------+------------+---------------------+---------------------+---------------------+\n";

    // Encode and write to output file
    std::ofstream outputFile("encoded_output.txt", std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Ошибка открытия файла для записи." << std::endl;
        return;
    }

    file.open(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Ошибка открытия файла для кодирования: " << filename << std::endl;
        return;
    }

    std::string encodedData;
    // system("chcp 866 > nul");
    while (file.get(ch))
    {
        encodedData += codeMap[(int)ch];
    }

    for (size_t i = 0; i < encodedData.size(); i += 8)
    {
        std::bitset<8> bits(encodedData.substr(i, 8));
        outputFile.put(static_cast<char>(bits.to_ulong()));
    }

    file.close();
    outputFile.close();

    // Calculate entropy and average code length
    double entropy = 0.0;
    for (const auto &pair : frequencies)
    {
        double p = static_cast<double>(pair.second) / totalSymbols;
        entropy -= p * log2(p);
    }

    double averageCodeLength = 0.0;
    for (const auto &pair : frequencies)
    {
        double p = static_cast<double>(pair.second) / totalSymbols;
        averageCodeLength += p * codeMap[pair.first].length();
    }

    system("chcp 65001 > nul");
    // Display entropy, average code length, and total encoded symbols
    int uniqueEncodedSymbols = frequencies.size();
    std::cout << "\nЭнтропия файла: " << entropy << std::endl;
    std::cout << "Средняя длина кодового слова: " << averageCodeLength << std::endl;
    std::cout << "Количество закодированных символов: " << uniqueEncodedSymbols << std::endl;

    // Free Huffman tree memory
    freeHuffmanTree(root);

    std::cout << "\nНажмите любую клавишу для возврата в меню...\n";
    system("pause");
}

int main()
{

    SetConsoleOutputCP(1251);

    system("chcp 65001 > nul");

    std::ifstream inFile("testBase3.dat", std::ios::binary);

    if (!inFile)
    {
        std::cerr << "Ошибка открытия файла\n";
        return 1;
    }

    inFile.seekg(0, std::ios::end);
    int fileSize = inFile.tellg();
    int totalRecords = fileSize / sizeof(Record);
    inFile.seekg(0, std::ios::beg);

    Record *records = new Record[totalRecords];

    inFile.read(reinterpret_cast<char *>(records), fileSize);
    inFile.close();

    Record **indexArray = new Record *[totalRecords];
    for (int i = 0; i < totalRecords; ++i)
    {
        indexArray[i] = &records[i];
    }

    Record **indexArrayKey;
    int index;
    int amount = 0;

    tree *p = nullptr;
    int coutFound;

    int e = 0, n = 0, page = 1;

    while (e != 1)
    {
        printPage(n, indexArray, totalRecords);
        std::cout << "Страница " << page << "\n";
        std::cout << "1 - Выход, 0 - Следующая страница, 9 - Предыдущая страница, 2 - Сортировать, 4 - Восстановить БД, 5 - Найти запись по номеру, 6 - Двоичный поиск по ключу, \n 7 - АВЛ-дерево, 8 - Кодировка :";
        std::cin >> e;

        if (e == 0)
        {
            n += 20;
            if (n >= totalRecords)
            {
                std::cout << "Достигнут конец записей.\n";
                n -= 20;
            }
            else
            {
                page++;
            }
        }
        else if (e == 9)
        {
            if (n >= 20)
            {
                n -= 20;
                page--;
            }
            else
            {
                std::cout << "Это первая страница.\n";
            }
        }
        else if (e == 2)
        {
            HeapSort(indexArray, totalRecords); // Сортировка по ключу K, вкладу и его
            std::cout << "Нажмите любую клавишу для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
        else if (e == 4)
        {
            for (int i = 0; i < totalRecords; ++i)
            {
                indexArray[i] = &records[i];
            }
            std::cout << "База данных восстановлена в исходное состояние.\n";
            std::cout << "Нажмите любую клавишу для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
        else if (e == 5)
        {
            std::cout << "Введите номер записи: ";
            int recordNumber;
            std::cin >> recordNumber;
            if (recordNumber >= 1 && recordNumber <= totalRecords)
            {
                n = (recordNumber - 1) / 20 * 20; // Определяем страницу
                page = (n / 20) + 1;
                printPage(n, indexArray, totalRecords);
                std::cout << "Страница " << page << "\n";
            }
            else
            {
                std::cout << "Неверный номер записи.\n";
            }
            std::cout << "Нажмите любую клавишу для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
        else if (e == 6)
        {
            HeapSort(indexArray, totalRecords);
            int depositToFind;
            std::cout << "Введите ключ (сумма вклада): ";
            std::cin >> depositToFind;
            index = BinarySearch(indexArray, totalRecords, depositToFind);
            amount = findRecordsAmount(indexArray, totalRecords, depositToFind, amount);
            indexArrayKey = new Record *[amount];
            indexArrayKey = requiredRecords(indexArray, indexArrayKey, index, amount);

            int flag = 1;
            int choice = 0;
            int nKey = 0;
            int pageKey = 0;

            while (flag)
            {
                printPage(nKey, indexArrayKey, amount);
                std::cout << "Страница " << pageKey << "\n";
                std::cout << "0 - Выход, 1 - Следующая страница, 2 - Предыдущая страница: ";
                std::cin >> choice;
                switch (choice)
                {
                case 1:
                    n += 20;
                    if (n >= totalRecords)
                    {
                        std::cout << "Достигнут конец записей.\n";
                        n -= 20;
                    }
                    else
                    {
                        page++;
                    }

                case 2:
                    if (n >= 20)
                    {
                        n -= 20;
                        page--;
                    }
                    else
                    {
                        std::cout << "Это первая страница.\n";
                    }
                    break;

                case 0:
                    flag = 0;
                    break;

                default:
                    flag = 0;
                    break;
                }
            }
            std::cout << "Нажмите любую клавишу для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
        else if (e == 7)
        {
            p = buildAVLTree(indexArrayKey, amount);

            printTableHeader();
            inOrderTraversal(p);

            char advocateKey[4];
            std::cout << "Введите Фамилию Адвоката (первые 3 буквы): ";
            system("chcp 866 > nul");
            std::cin >> advocateKey;
            system("chcp 65001 > nul");

            printTableHeader();
            coutFound = SearchInTree(p, 1, advocateKey);
            if (coutFound == 1)
            {
                std::cout << "Записей с такой фамилией не найдено\n";
            }

            std::cout << "Нажмите любую клавишу для продолжения...\n";
            std::cin.ignore();
            std::cin.get();
        }
        else if (e == 8)
        {
            // system("chcp 866 > nul");
            encodeMenuOption();
        }
    }
    delete[] records;
    delete[] indexArray;

    return 0;
}