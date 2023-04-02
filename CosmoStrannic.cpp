#define _CRT_SECURE_NO_WARNINGS
#include "conio.h"
#include "windows.h"
#include "ctime"
#include <iostream>

using namespace std;

struct user { // данные о пользователе (рекордн.таблица)
    char name[10];
    int record;
    int day;
    int month;
    int year;
};

struct sohr { // хранение данных о незавершенной игре
    int posGun;
    int timer;
    int itogOchky;
    int lifes;
    char mas[18][60];
};

void vyvodVkonsol(char mas[18][60]) { // поэлементный вывод в косоль 
    system("cls");
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 60; j++)
            cout << mas[i][j];
        if (i != 17)
            cout << endl;
    }
}

void menuIgra(int switcher); // подменю "Играть"
void glavnoeMenu(int switcher); // гл. меню
void nachaloIgry(char mas[18][60], int lifes, int* timer); // ф-ция, опред. начальный символьный массив при запуске нового игрового сеанса
void Igra(int var); // игровой сеанс
void konecIgry(int record); // ф-ция, спрашивающая игрока, и запис. его результ. в бинарный файл
void pomosh(int switcher); // помощь польз-лю
void zalSlavy(); // отображает список лидеров

void glavnoeMenu(int switcher) {
    system("cls");
    switch (switcher)
    {
    case 1:
        cout << "\n\n\n\n\n\n                        <<  ИГРАТЬ  >>\n\n                            ПОМОЩЬ\n\n                           ЗАЛ СЛАВЫ\n\n                            ВЫХОД";
        break;
    case 2:
        cout << "\n\n\n\n\n\n                            ИГРАТЬ\n\n                        <<  ПОМОЩЬ  >>\n\n                           ЗАЛ СЛАВЫ\n\n                            ВЫХОД";
        break;
    case 3:
        cout << "\n\n\n\n\n\n                            ИГРАТЬ\n\n                            ПОМОЩЬ\n\n                        << ЗАЛ СЛАВЫ >>\n\n                            ВЫХОД";
        break;
    case 4:
        cout << "\n\n\n\n\n\n                            ИГРАТЬ\n\n                            ПОМОЩЬ\n\n                           ЗАЛ СЛАВЫ\n\n                       <<   ВЫХОД   >>";
        break;
    }
    int choice = _getch();
    if (choice == 224)
        choice = _getch();
    if (choice == 72)
        if (switcher != 1)
            glavnoeMenu(switcher - 1);
        else
            glavnoeMenu(4);
    if (choice == 80)
        if (switcher != 4)
            glavnoeMenu(switcher + 1);
        else
            glavnoeMenu(1);
    if (choice == 13 || choice == 32)
    {
        if (switcher == 1)
            menuIgra(1);
        if (switcher == 2)
            pomosh(0);
        if (switcher == 3)
            zalSlavy();
        if (switcher == 4)
            _exit(0);
    }
}

void menuIgra(int switcher) {
    system("cls");
    if (switcher == 1)
        cout << "\n\n\n\n\n\n\n\n                      <<  НОВАЯ ИГРА  >>\n\n                          ПРОДОЛЖИТЬ";
    else
        cout << "\n\n\n\n\n\n\n\n                          НОВАЯ ИГРА\n\n                      <<  ПРОДОЛЖИТЬ  >>";
    int choice = _getch();
    if (choice == 224)
        choice = _getch();
    if (choice == 72 || choice == 80)
        menuIgra(3 - switcher);
    if (choice == 27)
        glavnoeMenu(1);
    if (choice == 13 || choice == 32)
        Igra(switcher);
}

void nachaloIgry(char mas[18][60], int lifes, int* timer) {
    for (int i = 0; i < 18; i++) // очистка от мусора
        for (int j = 0; j < 60; j++)
            mas[i][j] = ' ';
    for (int i = 60; lifes > 0; i--, lifes--, mas[0][i] = '%');
    *timer = 50;
    char bokovyeSimvoly[] = { '†', '‡', '¤', ' ' };
    for (int pole = 0; pole < 60; pole++) // прорисовка верхнего и нижнего поля
    {
        mas[1][pole] = bokovyeSimvoly[rand() % 3];
        if (mas[1][pole] == '‡')
            mas[2][pole] = '¤';

        mas[17][pole] = bokovyeSimvoly[rand() % 3];
        if (mas[17][pole] == '‡')
            mas[16][pole] = '¤';
    }
    mas[8][0] = '\\'; mas[8][1] = '\\'; // корабль
    mas[9][0] = '#'; mas[9][1] = '$'; mas[9][2] = '=';
    mas[10][0] = '/'; mas[10][1] = '/';
}

void konecIgry(int record) {
    system("cls");
    user newUser; //объявляние структуры
    newUser.record = record; // поле для набранного счета
    cout << "Поздравляем Вас!\nВы продержались " << record << " километров.\n\n(Пожалуйста, не используйте кириллические символы)\n(Используйте не более 6 символов)\nОставьте свое имя и станьте примером\nдля подражания будущим игрокам: ";
    cin.getline(newUser.name, 10); // поле имени
    time_t timeCur;
    time(&timeCur);
    struct tm* timeCur18uct = localtime(&timeCur);
    newUser.day = timeCur18uct->tm_mday; // дата завершения игры
    newUser.month = timeCur18uct->tm_mon;
    newUser.year = timeCur18uct->tm_year;
    FILE* ZalSlavy;
    fopen_s(&ZalSlavy, "zalSlavy.bin", "ab+");
    fwrite(&newUser, 1, sizeof(user), ZalSlavy); // дозапись результата в файл
    fclose(ZalSlavy);
    zalSlavy();
}

void zalSlavy()
{
    FILE* ZalSlavy;
    fopen_s(&ZalSlavy, "zalSlavy.bin", "rb+");
    system("cls");
    if (ZalSlavy == NULL) { // ошибка при открытии файла
        system("cls");
        cout << "Нет ни единого результата.";
        Sleep(500);
        system("cls");
        cout << "Нет ни единого результата..";
        Sleep(500);
        system("cls");
        cout << "Нет ни единого результата...";
        Sleep(500);
        cout << "\nНажмите любую клавишу, чтобы вернуться.";
        _getch();
        glavnoeMenu(3);
    }
    fseek(ZalSlavy, 0L, SEEK_END);
    int kolvoIgrokov = ftell(ZalSlavy) / sizeof(user);
    user* tmp = new user[kolvoIgrokov];
    fseek(ZalSlavy, 0L, SEEK_SET);
    for (int i = 0; i < kolvoIgrokov; i++) // копирование содержиомого файла в структкры
        fread(&tmp[i], 1, sizeof(user), ZalSlavy);
    fclose(ZalSlavy);
    for (int i = 1; i < kolvoIgrokov; i++) // сортировка структур по рейтингу счета
        if (tmp[i].record > tmp[i - 1].record)
        {
            user tmpOdin;
            strcpy(tmpOdin.name, tmp[i].name);
            tmpOdin.record = tmp[i].record;
            tmpOdin.day = tmp[i].day;
            tmpOdin.month = tmp[i].month;
            tmpOdin.year = tmp[i].year;

            strcpy(tmp[i].name, tmp[i - 1].name);
            tmp[i].record = tmp[i - 1].record;
            tmp[i].day = tmp[i - 1].day;
            tmp[i].month = tmp[i - 1].month;
            tmp[i].year = tmp[i - 1].year;

            strcpy(tmp[i - 1].name, tmpOdin.name);
            tmp[i - 1].record = tmpOdin.record;
            tmp[i - 1].day = tmpOdin.day;
            tmp[i - 1].month = tmpOdin.month;
            tmp[i - 1].year = tmpOdin.year;

            if (i > 1)
                i -= 2;
            else
                i = 0;
        }
    if (kolvoIgrokov > 12)
        kolvoIgrokov = 12;
    cout << "№       " << "Имя" << '\t' << "Счет" << '\t' << "Дата" << endl;//вывод таблицы лидеров в консоль
    for (int i = 0; i < kolvoIgrokov; i++)
    {
        cout << i + 1 << ')' << '\t' << tmp[i].name << '\t' << tmp[i].record << '\t';
        if (tmp[i].day / 10 == 0)
            cout << '0' << tmp[i].day;
        else
            cout << tmp[i].day;
        cout << ' ';
        switch (tmp[i].month)
        {
        case 0:
            cout << "января";
            break;
        case 1:
            cout << "февраля";
            break;
        case 2:
            cout << "марта";
            break;
        case 3:
            cout << "апреля";
            break;
        case 4:
            cout << "мая";
            break;
        case 5:
            cout << "июня";
            break;
        case 6:
            cout << "июля";
            break;
        case 7:
            cout << "августа";
            break;
        case 8:
            cout << "сентября";
            break;
        case 9:
            cout << "октября";
            break;
        case 10:
            cout << "ноября";
            break;
        case 11:
            cout << "декабря";
            break;
        }
        cout << ' ' << 1900 + tmp[i].year << endl;
    }
    fopen_s(&ZalSlavy, "zalSlavy.bin", "wb+");
    for (int i = 0; i < kolvoIgrokov; i++) // запись таблицы лидеров в бинарный файл
        fwrite(&tmp[i], 1, sizeof(user), ZalSlavy);
    fclose(ZalSlavy);
    delete[] tmp;
    _getch();
    glavnoeMenu(3);
}

void pomosh(int switcher)
{
    system("cls");
    cout << "ПРОКРУТКА: СТРЕЛКИ ВВЕРХ/ВНИЗ | ВЕРНУТЬСЯ: ESCAPE\n";
    char mas[2000] = { "                     НАВИГАЦИЯ ПО МЕНЮ:                       Выбрать пункт - ПРОБЕЛ или ENTER                            Передвижение по пунктам меню - стрелки ВВЕРХ и ВНИЗ         Вернуться в предыдущее меню - ESCAPE                                                                                                       УПРАВЛЕНИЕ В ИГРЕ:                       Сделать выстрел - ПРОБЕЛ                                    Передвигаться направо и налево - стрелки ПРАВО/ЛЕВО         Вернуться в меню с сохраненеим - ESCAPE                                                                                                         О ИГРЕ:                             Вы - космический странник, изучающий просторы Вселенной. В ходе своего путешествия Вы сбились с пути и попали в <<Пояс астероидов>>. Вам предстоит пробиваться сквозь нескончаемый поток осколков, сбивая их с помощью лазерной пушки, встро - енной в Ваше судно! В верхней части экрана экрана располо - жен <<щиток приборов>> - панель, на которой отображено ко - личество оставшихся жизней (символ < % >), текущая и прой - денная скорость в КМ/мин (километров в минуту).              В любой момент Вы можете закончить игру - просто нажмите   ESC, игра автоматически сохранится и Вы сможете в любой мо- мент продолжить ее, нажав <<ПРОДОЛЖИТЬ>>                     Посмотрите в главном меню список лидеров! Старайтесь, и    когда-то Вы займете почетное место среди уважаемых пилотов!                                                                                        АВТОР:                                                  Андрей Климшин                                                   2023" };
    for (int i = 0, buf = switcher; i < 17; i++)
    {
        for (int j = buf * 60; j < buf * 60 + 60; j++)
            cout << mas[j];
        if (i != 16)
            cout << endl;
        buf++;
    }
    int klavisha = _getch(); // получить значение надатой клавиши
    if (klavisha == 224) // если стрелка
        klavisha = _getch(); // определить какая
    if (klavisha == 72) // вверх
        if (switcher > 0)
            pomosh(switcher - 1);
        else
            pomosh(0);
    if (klavisha == 80) // вниз
        if (switcher < 22)
            pomosh(switcher + 1);
        else
            pomosh(22);
    if (klavisha == 27) // Escape
        glavnoeMenu(2);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    system("mode con cols=61 lines=18");
    system("title Космо Странник");
    system("color 64"); // цвет консоли (6-задний фон; 4-передний фон)
    HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE); // получение хендла
    CONSOLE_CURSOR_INFO cursor = { 100, false }; // число от 1 до 100 размер курсора в процентах; false\true - видимость
    SetConsoleCursorInfo(hCons, &cursor); // применение заданных параметров курсора
    int timer = 270;
    Sleep(timer);
    system("cls");
    cout << "\n              __  __  ____  ____  __  __  ____\n             |  |/  )(    )|  __)(  \\/  )(    )\n             |     (  ) O( | |__  ) __ (  ) O(\n             |__|\\__)(____)|____)(__)(__)(____)\n   ____  ____  ____   __   __   __  __   __  __  __  __  __\n  |  __)(_  _)(  __\\ / _\\ (  )_(  )(  )_(  )(  |/  ||  \\/  )\n  | |__   )(   | __//    \\|   _   ||   _   | )    ( |     (\n  |____) (__) (__)  \\_/\\_/(__) (__)(__) (__)(__/|__)|__|\\__)";
    Sleep(timer);
    system("cls");
    cout << "\n\n              __  __  ____  ____  __  __  ____\n             |  |/  )(    )|  __)(  \\/  )(    )\n             |     (  ) O( | |__  ) __ (  ) O(\n             |__|\\__)(____)|____)(__)(__)(____)\n   ____  ____  ____   __   __   __  __   __  __  __  __  __\n  |  __)(_  _)(  __\\ / _\\ (  )_(  )(  )_(  )(  |/  ||  \\/  )\n  | |__   )(   | __//    \\|   _   ||   _   | )    ( |     (\n  |____) (__) (__)  \\_/\\_/(__) (__)(__) (__)(__/|__)|__|\\__)";
    Sleep(timer);
    system("cls");
    cout << "\n\n\n              __  __  ____  ____  __  __  ____\n             |  |/  )(    )|  __)(  \\/  )(    )\n             |     (  ) O( | |__  ) __ (  ) O(\n             |__|\\__)(____)|____)(__)(__)(____)\n   ____  ____  ____   __   __   __  __   __  __  __  __  __\n  |  __)(_  _)(  __\\ / _\\ (  )_(  )(  )_(  )(  |/  ||  \\/  )\n  | |__   )(   | __//    \\|   _   ||   _   | )    ( |     (\n  |____) (__) (__)  \\_/\\_/(__) (__)(__) (__)(__/|__)|__|\\__)";
    Sleep(timer);
    system("cls");
    cout << "\n\n\n\n              __  __  ____  ____  __  __  ____\n             |  |/  )(    )|  __)(  \\/  )(    )\n             |     (  ) O( | |__  ) __ (  ) O(\n             |__|\\__)(____)|____)(__)(__)(____)\n   ____  ____  ____   __   __   __  __   __  __  __  __  __\n  |  __)(_  _)(  __\\ / _\\ (  )_(  )(  )_(  )(  |/  ||  \\/  )\n  | |__   )(   | __//    \\|   _   ||   _   | )    ( |     (\n  |____) (__) (__)  \\_/\\_/(__) (__)(__) (__)(__/|__)|__|\\__)";
    Sleep(timer);
    system("cls");
    cout << "\n\n\n\n\n              __  __  ____  ____  __  __  ____\n             |  |/  )(    )|  __)(  \\/  )(    )\n             |     (  ) O( | |__  ) __ (  ) O(\n             |__|\\__)(____)|____)(__)(__)(____)\n   ____  ____  ____   __   __   __  __   __  __  __  __  __\n  |  __)(_  _)(  __\\ / _\\ (  )_(  )(  )_(  )(  |/  ||  \\/  )\n  | |__   )(   | __//    \\|   _   ||   _   | )    ( |     (\n  |____) (__) (__)  \\_/\\_/(__) (__)(__) (__)(__/|__)|__|\\__)";

    Sleep(10 * timer);
    glavnoeMenu(1);
    return 0;
}

void Igra(int var)
{
    int posGun; 
    int timer; 
    int itogOchky; 
    int lifes; 
    char control = '&'; 
    int zaderjVystrl = 4; 
    int speed; 
    char mas[18][60];
    if (var == 1)
    {
        posGun = 9;
        itogOchky = 1;
        lifes = 3;
        nachaloIgry(mas, lifes, &timer);
    }
    else // при восстановлении из сохранения
    {
        FILE* sohrBin;
        fopen_s(&sohrBin, "Currentsohr.bin", "rb");
        if (!sohrBin)
        {
            system("cls");
            cout << "Нет сохранения.";
            Sleep(1000);
            system("cls");
            cout << "Нет сохранения..";
            Sleep(1000);
            system("cls");
            cout << "Нет сохранения...";
            Sleep(1000);
            Igra(1);
        }
        fread(&posGun, 1, sizeof(int), sohrBin);
        timer = 80;
        fread(&itogOchky, 1, sizeof(int), sohrBin);
        fread(&lifes, 1, sizeof(int), sohrBin);
        fread(&mas, 18 * 60, sizeof(char), sohrBin);
        fclose(sohrBin);
        remove("Currentsohr.bin");
    }
    while (true)
    {
        int bufItogOchkov = itogOchky, dlinaItogOchkov;
        for (dlinaItogOchkov = 0; bufItogOchkov != 0; bufItogOchkov /= 10, dlinaItogOchkov++); // вычисление количества цифр под лину итоговых очков
        for (int i = dlinaItogOchkov, bufItogOchkov = itogOchky; i >= 0; i--, mas[0][i] = bufItogOchkov % 10 + '0', bufItogOchkov /= 10); // прорисовка итог очков на приборн.панель
        mas[0][dlinaItogOchkov++] = 'К'; mas[0][dlinaItogOchkov++] = 'М'; // дописывание "КМ"
        itogOchky++; // пополнение итог.очков
        speed = 1000 / timer; // обновление спидометра
        int speedBuf = speed;
        for (int i = 52; speed != 0; i--, mas[0][i] = speed % 10 + '0', speed /= 10); // прорисовка спидометра на приборную панель
        mas[0][50] = 'К'; mas[0][51] = 'М'; mas[0][52] = '/'; mas[0][53] = 'м'; mas[0][54] = 'и'; mas[0][55] = 'н'; // дописывание "КМ/мин"
        if (_kbhit()) // если нажата клавиша
        {
            control = _getch(); // принять значение
            if (control == 224)
                control = _getch();
        }
        if (control == 17 && zaderjVystrl == 4 || control == 32 && zaderjVystrl == 4) // при нажатии на курок, если заряжен
        {
            mas[posGun][3] = '~';
            zaderjVystrl = 0;
        }
        if (zaderjVystrl < 4) // перезарядка
            zaderjVystrl++;
        if (control == 27) // при выходе
        {
            FILE* sohrBin;
            fopen_s(&sohrBin, "Currentsohr.bin", "wb");
            fwrite(&posGun, 1, sizeof(int), sohrBin);
            fwrite(&itogOchky, 1, sizeof(int), sohrBin);
            fwrite(&lifes, 1, sizeof(int), sohrBin);
            fwrite(&mas, 18 * 60, sizeof(char), sohrBin);
            fclose(sohrBin);
            menuIgra(2);
        }
        if (control == 72) // при движении корабля вверх
            if (mas[2][0] == '\\' || mas[3][0] == '\\' && mas[2][0] == '¤' || mas[3][1] == '\\' && mas[2][1] == '¤')//если корабль врезался в верхнее поле - игра окончена
                if (lifes > 1)
                {
                    cout << '\a';
                    lifes--;
                    posGun = 9;
                    nachaloIgry(mas, lifes, &timer);
                    Sleep(1000);
                }
                else
                    konecIgry(itogOchky);
            else
            {
                for (int i = 2; i < 17; i++) // корабль смещается на элемент выше
                    for (int j = 0; j < 59; j++)
                        if (mas[i][j] == '#' || mas[i][j] == '\\' || mas[i][j] == '$' || mas[i][j] == '=' || mas[i][j] == '/')
                        {
                            mas[i - 1][j] = mas[i][j];
                            mas[i][j] = ' ';
                        }
                posGun--;
            }
        if (control == 80) // при движении корабля вниз
            if (mas[16][0] == '/' || mas[15][0] == '/' && mas[16][0] == '¤' || mas[15][1] == '/' && mas[16][1] == '¤') // если корабль врезался в нижнее поле - игра окончена
                if (lifes > 1)
                {
                    cout << '\a';
                    lifes--;
                    posGun = 9;
                    nachaloIgry(mas, lifes, &timer);
                    Sleep(1000);
                }
                else
                    konecIgry(itogOchky);
            else
            {
                for (int i = 16; i >= 2; i--) // корабль смещается на элемент вниз
                    for (int j = 0; j < 59; j++)
                        if (mas[i][j] == '#' || mas[i][j] == '\\' || mas[i][j] == '$' || mas[i][j] == '=' || mas[i][j] == '/')
                        {
                            mas[i + 1][j] = mas[i][j];
                            mas[i][j] = ' ';
                        }
                posGun++;
            }
        for (int i = 1; i < 18; i++) // все осколки планет смещаются на элемент вниз
            for (int j = 0; j < 59; j++)
            {
                if (mas[i][j] == '\\' && mas[i][j + 1] == '¤' || mas[i][j] == '=' && mas[i][j + 1] == '¤' || mas[i][j] == '/' && mas[i][j + 1] == '¤')
                    if (lifes > 1)
                    {
                        cout << '\a';
                        lifes--;
                        posGun = 9;
                        nachaloIgry(mas, lifes, &timer);
                        Sleep(1000);
                    }
                    else
                        konecIgry(itogOchky);
                if (mas[i][j] != '#' && mas[i][j] != '\\' && mas[i][j] != '$' && mas[i][j] != '=' && mas[i][j] != '/' && mas[i][j] != '~' && mas[i][j + 1] != '~')
                    mas[i][j] = mas[i][j + 1];
                if (mas[i][j] == '¤')
                    mas[i][j + 1] = ' ';
            }
        for (int i = 1; i < 18; i++) // все снаряды смещаются на элемент вправо
            for (int j = 58; j >= 0; j--)
                if (mas[i][j] == '~')
                    if (j != 58)
                    {
                        mas[i][j + 1] = '~';
                        mas[i][j] = ' ';
                    }
                    else
                        mas[i][j] = ' ';
        char bokovyeSimvoly[] = { '†', '‡', '¤', ' ' };
        mas[2][59] = ' '; // рандомное заполнение новых элементов краев
        mas[1][59] = bokovyeSimvoly[rand() % 3];
        if (mas[1][59] == '‡')
            mas[2][59] = '¤';
        mas[16][59] = ' ';
        mas[17][59] = bokovyeSimvoly[rand() % 3];
        if (mas[17][59] == '‡')
            mas[16][59] = '¤';
        for (int i = 3; i < 16; i++) // рандомное появление осколков планет
        {
            if (rand() % 10 == 1)
                mas[i][59] = '¤';
        }
        vyvodVkonsol(mas); // вывод экрана
        if (control != '&') // обнуление управляющей переменной
            control = '&';
        if (timer > 25) // ускорение
            timer--;
        Sleep(timer); // задержка перерисовки
    }
}