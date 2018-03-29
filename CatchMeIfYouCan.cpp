/*!
 * \file main.cpp
 * \brief Projet C++ : modifications apportées au jeu.
 * \author Axel GATINEAU PHILIPPE, Julie FARDEAU, Lucas GARCIA--ROTA, Maxime DECORDE, Olivier GOASAMPIS
 */

#include<iostream>
#include<iomanip> //! accès à setw()
#include<cstdlib> //! accès à rand()
#include<vector>
#include<string>
#include<cctype> //! accès à toupper ()
#include<utility> //! accès au type pair
#include<map> //! accès au type name
#include<fstream> //! manipulation des fichiers
#include<unistd.h>//! includes pour la fonction set_input_mode
#include<termios.h>

using namespace std;

/*!
 * \namespace
 * Espace de nommage regroupant les fonctions appellées par le main.
 */
namespace{

    //! 27. Un type représentant une ligne de la grille.
    typedef vector <char> CVLine;
    //! 29. Un type représentant la grille.
    typedef vector <CVLine> CMatrix;
    //! 31. Un type représentant une coordonnée dans la grille.
    typedef pair   <unsigned, unsigned> CPosition;
    //! 33. Un type utilisé pour stocker le nom et la couleur des joueurs.
    typedef pair   <string, string> CPairString;
    //! 35. Un type utilisé pour faire des vecteurs de pairs (ici 2 strings).
    typedef vector <CPairString> CVPairStr;

    //! 38. Pion du joueur 1.
    char TokenPlayer1 = 'X';
    //! 40. Pion du joueur 2.
    char TokenPlayer2 = 'O';
    //! 42. Case vide.
    const char KEmpty          = '_';
    //! 44. Pion du carré rouge.
    const char KTokenRedSquare = '#';
    //! 46. Pion de la pièce.
    const char KTokenCoin      = '+';

    //! 49. Alias pour rétablir la couleur.
    const string KReset   ("0");
    //! 51. Alias pour la couleur noir.
    const string KNoir    ("30");
    //! 53. Alias pour la couleur rouge.
    const string KRouge   ("31");
    //! 55. Alias pour la couleur vert.
    const string KVert    ("32");
    //! 57. Alias pour la couleur jaune.
    const string KJaune   ("33");
    //! 59. Alias pour la couleur bleu.
    const string KBleu    ("34");
    //! 61. Alias pour la couleur magenta.
    const string KMagenta ("35");
    //! 63. Alias pour la couleur cyan.
    const string KCyan    ("36");

    struct termios saved_attributes;

    /*!
     * \fn void reset_input_mode (void)
     * \brief Bascule en mode canonique.
     */
    void reset_input_mode (void){
          tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
    } // reset_input_mode()

    /*!
     * \fn void set_input_mode (void)
     * \brief Bascule en mode non canonique.
     */
    //! http://www.gnu.org/software/libc/manual/html_node/Noncanon-Example.html
    void set_input_mode (void){
        struct termios tattr;
        //! 83. Make sure stdin is a terminal.
        if (!isatty (STDIN_FILENO)){
            fprintf (stderr, "Not a terminal.\n");
            exit (EXIT_FAILURE);
        }
        //! 88. Save the terminal attributes so we can restore them later.
        tcgetattr (STDIN_FILENO, &saved_attributes);
        atexit (reset_input_mode);
        //! 91. Set the funny terminal modes.
        tcgetattr (STDIN_FILENO, &tattr);
		//! 93. Clear ICANON and ECHO.
        tattr.c_lflag &= ~(ICANON|ECHO); 
        tattr.c_cc[VMIN] = 0;
        tattr.c_cc[VTIME] = 200;
        tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
    } // set_input_mode()

    /*!
     * \fn void ClearScreen ()
     * \brief Efface ce qui est affiché sur le terminal.
     */
    void ClearScreen (){
        cout << "\033[H\033[2J";
    } // ClearScreen ()

    /*!
     * \fn void ShowPrompt (T & var)
     * \brief Attend la saisie de l'utilisateur de la valeur d'une variable passée en paramètre, en affichant un point d'interrogation.
     * \param var a un type générique, cette fonction marche donc pour toutes les saisies.
     */
    template <typename T>
    void ShowPrompt (T & var){
        cout << endl << "? ";
        cin >> var;
        if (cin.fail ()){
            cin.clear ();
            cin.ignore ();
            ShowPrompt (var);
        }
    } // ShowPrompt ()

    /*!
     * \fn Modifie la couleur de l'affichage.
     * \param Le paramètre est l'une des constantes de couleur déclarées dans le namespace.
     */
    void Couleur (const string & coul){
        cout << "\033[" << coul << "m";
    } // Couleur ()

    /*!
     * \fn ShowColoredName (CPairString NameColor)
     * \brief Affiche un pseudo de la couleur qui lui est associée.
     * \param NameColor est un élément du vecteur VNameColor.
     */
    void ShowColoredName (CPairString & NameColor){
        Couleur (NameColor.second);
        cout << NameColor.first << " ";
        Couleur (KReset);
    } // ShowColoredName ()

    /*!
     * \fn string ColorChoose (string & color)
     * \brief Propose à l'utilisateur de sélectionner la couleur de son pion.
     * \param color sera associé à un pseudo dans VNameColor.
     */
    string ColorChoose (string & color){
        unsigned choice;

        cout << "Choisir sa couleur :" << endl
             << "1. ";
        Couleur (KRouge);
        cout << "Rouge";
        Couleur (KReset);

        cout << endl << "2. ";
        Couleur (KVert);
        cout << "Vert";
        Couleur (KReset);

        cout << endl << "3. ";
        Couleur (KJaune);
        cout << "Jaune";
        Couleur (KReset);

        cout << endl << "4. ";
        Couleur (KBleu);
        cout << "Bleu";
        Couleur (KReset);

        cout << endl << "5. ";
        Couleur (KMagenta);
        cout << "Magenta";
        Couleur (KReset);

        cout << endl << "6. ";
        Couleur (KCyan);
        cout << "Cyan";
        Couleur (KReset);
        cout << endl;

        ShowPrompt (choice);
        cout << endl;

        switch (choice){
            case 1:
                color = KRouge;
                break;
            case 2:
                color = KVert;
                break;
            case 3:
                color = KJaune;
                break;
            case 4:
                color = KBleu;
                break;
            case 5:
                color = KMagenta;
                break;
            case 6:
                color = KCyan;
                break;
        }
        return color;
    } // ColorChoose ()

    /*!
     * \fn void InitMat (CMatrix & mat, const unsigned & matrixSize)
     * \brief Dimmensionne la matrice à la valeur de matrixSize. On peut noter que la matrice doit être carrée, donc une seule variable suffit.
     */
    void InitMat (CMatrix & mat, const unsigned & matrixSize){
        mat.resize(matrixSize);
        for (unsigned i(0); i < mat.size(); ++i)
            mat[i].resize(matrixSize);
    }// InitMat()

    /*!
     * \fn void ShowMatrix (const CMatrix & mat, const CVPairStr & VNameColor, const vector<unsigned> & VScore, const unsigned & playersCouple)
     * \brief Affiche la matrice et son contenu à l'écran.
     * \param playersCouple est un entier qui définit un "couple" de joueurs qui s'affrontent. VNameColor à l'indice playersCouple correspond au joueur 1 et l'indice playersCouple+1 au joueur 2.
     * Affiche la matrice et son contenu à l'écran.
     */
    void ShowMatrix (const CMatrix & mat, const CVPairStr & VNameColor, const vector<unsigned> & VScore, const unsigned & playersCouple){
        ClearScreen();
        Couleur (KBleu);
        cout << "\t# Deplacements" << endl;
        Couleur (KReset);
        cout << "\t A   Z   E" << endl << "\t     ^" << endl << "\tQ  < o >  D " << endl << "\t     v" << endl << "\t W   X   C" << endl << endl; //! Affiche un dessin explicitant les déplacement possibles, correspondant aux lettres.

        //! Affiche les pseudos en couleur des joueurs qui s'affrontent.
        cout << endl << '\t';
        Couleur (VNameColor [playersCouple].second);
        cout << VNameColor [playersCouple].first;
        Couleur (KReset);

        cout << "\t\t" << "vs." << "\t\t";

        Couleur (VNameColor [playersCouple + 1].second);
        cout << VNameColor [playersCouple + 1].first;
        Couleur (KReset);

        //! Affiche leur score actuel.
        Couleur (KBleu);
        cout << endl << "Score : ";
        Couleur (KReset);

        Couleur (VNameColor [playersCouple].second);
        cout << VScore [playersCouple];
        Couleur (KReset);

        cout << "\t\t" << "/" << "\t\t";

        Couleur (VNameColor [playersCouple + 1].second);
        cout << VScore [playersCouple + 1];
        Couleur (KReset);

        cout << endl << endl << endl;

        for (unsigned i(0); i < mat.size(); ++i){
            cout << "|";
            for (unsigned j(0); j < mat[i].size(); ++j){
                //! Quand les coordonnées [i][j] correspondent aux coordonnées des pions des joueurs ou du carré rouge, celui-ci est affiché de la bonne couleur. Sinon la case affichée est vide.
                if (mat[i][j] == TokenPlayer1){
                    Couleur (VNameColor[playersCouple].second);
                    cout << TokenPlayer1;
                    Couleur (KReset);
                    cout << '|';
                }
                else if (mat[i][j] == TokenPlayer2){
                    Couleur (VNameColor[playersCouple+1].second);
                    cout << TokenPlayer2;
                    Couleur (KReset);
                    cout << '|';
                }
                else if (mat[i][j] == KTokenRedSquare){
                    Couleur (KRouge);
                    cout << KTokenRedSquare;
                    Couleur (KReset);
                    cout << '|';
                }
                else if (mat[i][j] == KTokenCoin){
                    Couleur (KBleu);
                    cout << KTokenCoin;
                    Couleur (KReset);
                    cout << '|';
                }
                else{
                    cout << KEmpty;
                    cout << '|';
                }
            }
            cout << endl;
        }
        cout << endl << endl;
    } // ShowMatrix ()

    /*!
     * \brief Deplace le joueur, teste la validité et la possibilité du déplacement.
     * \param[in-out] mat matrice initialisée, redimmensionnée et affichée à l'écran dans laquelle les joueurs vont se déplacer
     * \param[in-out]caracère prenant une des valeurs présentes à coté de la matrice lors de l'affichage (Z,D,X,Q,A,E,C,W) et qui définira la déplacement du pion (en diagonale, en haut/bas, à gauche/droite)
     * \param[in-out] pos Position dans le vecteur, est modifiée lors d'un déplacement
     * \param[in] turn Tour actuel, 1 ou 2 pour le joueur 1 ou le joueur 2 respectivement.
     * \param[in] VNameColor tableau de pairs (ici, couple de deux chaines de caractères) associant les joueurs (leur nom) à une couleur
     * \param[in] playersCouple entier qui définit un "couple" de joueurs qui jouent dans un même duel
     */
    void MoveToken (CMatrix         & mat,
                    char            & move,
                    CPosition       & pos,
                    const unsigned  & turn,
                    const CVPairStr & VNameColor,
                    const unsigned  & playersCouple,
                    const unsigned  & matrixSize){
						
        //! Vérification de la validité du déplacement. Si le pion va sortir de la matrice, une nouvelle valeur de Move est demandée.
        while ((move == 'Z' && pos.first  == 0)            ||
               (move == 'D' && pos.second == matrixSize-1) ||
               (move == 'X' && pos.first  == matrixSize-1) ||
               (move == 'Q' && pos.second == 0)            ||
               (move == 'A' && pos.first  == 0)            ||
               (move == 'A' && pos.second == 0)            ||
               (move == 'E' && pos.first  == 0)            ||
               (move == 'E' && pos.second == matrixSize-1) ||
               (move == 'C' && pos.first  == matrixSize-1) ||
               (move == 'C' && pos.second == matrixSize-1) ||
               (move == 'W' && pos.first  == matrixSize-1) ||
               (move == 'W' && pos.second == 0)            ||
               (move == 'z' && pos.first  == 0)            ||
               (move == 'd' && pos.second == matrixSize-1) ||
               (move == 'x' && pos.first  == matrixSize-1) ||
               (move == 'q' && pos.second == 0)            ||
               (move == 'a' && pos.first  == 0)            ||
               (move == 'a' && pos.second == 0)            ||
               (move == 'e' && pos.first  == 0)            ||
               (move == 'e' && pos.second == matrixSize-1) ||
               (move == 'c' && pos.first  == matrixSize-1) ||
               (move == 'c' && pos.second == matrixSize-1) ||
               (move == 'w' && pos.first  == matrixSize-1) ||
               (move == 'w' && pos.second == 0)){
            cout << "Deplacement impossible." << endl;
            ShowPrompt (move);
        }

        //! Autre vérification. Si la valeur de Move ne correspondant à l'une des options possibles, une nouvelle valeur est demandée.
        while ((move != 'A') &&
               (move != 'Z') &&
               (move != 'E') &&
               (move != 'Q') &&
               (move != 'D') &&
               (move != 'W') &&
               (move != 'X') &&
               (move != 'C') &&
               (move != 'a') &&
               (move != 'z') &&
               (move != 'e') &&
               (move != 'q') &&
               (move != 'd') &&
               (move != 'w') &&
               (move != 'x') &&
               (move != 'c')){
            cout << "Saisie incorrecte." << endl;
            ShowPrompt (move);
        }

        //! Modifie les coordonnées du pion en fonction du déplacement exigé.
        mat[pos.first][pos.second] = KEmpty;

        switch (move){
            case 'A' :
                pos = make_pair(pos.first-1, pos.second-1);
                break;
            case 'Z' :
                pos = make_pair(pos.first-1, pos.second);
                break;
            case 'E' :
                pos = make_pair(pos.first-1, pos.second+1);
                break;
            case 'Q' :
                pos = make_pair(pos.first, pos.second-1);
                break;
            case 'D' :
                pos = make_pair(pos.first, pos.second+1);
                break;
            case 'W' :
                pos = make_pair(pos.first+1, pos.second-1);
                break;
            case 'X' :
                pos = make_pair(pos.first+1, pos.second);
                break;
            case 'C' :
                pos = make_pair(pos.first+1, pos.second+1);
                break;
            case 'a' :
                pos = make_pair(pos.first-1, pos.second-1);
                break;
            case 'z' :
                pos = make_pair(pos.first-1, pos.second);
                break;
            case 'e' :
                pos = make_pair(pos.first-1, pos.second+1);
                break;
            case 'q' :
                pos = make_pair(pos.first, pos.second-1);
                break;
            case 'd' :
                pos = make_pair(pos.first, pos.second+1);
                break;
            case 'w' :
                pos = make_pair(pos.first+1, pos.second-1);
                break;
            case 'x' :
                pos = make_pair(pos.first+1, pos.second);
                break;
            case 'c' :
                pos = make_pair(pos.first+1, pos.second+1);
                break;
        }
        mat[pos.first][pos.second] = ((turn == 1) ? (TokenPlayer1) : (TokenPlayer2));
    } // MoveToken ()

    /*!
     * \fn CVPairStr InitPlayers (const unsigned & mode)
     * \brief Initialise le vecteur contenant les pseudos des joueurs avec leur couleur.
     */
    CVPairStr InitPlayers (const unsigned & mode){
        ClearScreen();

        unsigned nb (1);
        string name, color;
        CVPairStr VNameColor;

        //! Mode duel.
        if (mode == 1){
            //! L'utilisateur entre le nom et choisit la couleur des deux seuls joueurs.
            for (;nb < 3; ++nb){
                ClearScreen();
                cout << "Nom du joueur " << nb << " : ";
                cin >> name;

                cout << endl;
                cout << "Saisir le caractère qui sera le pion du joueur " << ((nb == 1) ? ("1") : ("2")) << " : ";
                cin >> ((nb == 1) ? (TokenPlayer1) : (TokenPlayer2));

                cout << endl;
                ColorChoose (color);

                CPairString NameColor = make_pair (name, color);
                VNameColor.push_back (NameColor);
            }
        }

        //! Mode Multijoueur.
        else if (mode == 2){
            unsigned nbPlayers;
            cout << "Saisir le nombre de joueurs : ";
            ShowPrompt (nbPlayers);

            while (nbPlayers % 2 != 0){
                cout << "Veuillez saisir un nombre pair de joueurs : ";
                ShowPrompt (nbPlayers);
            }

            while (VNameColor.size() < nbPlayers){
                ClearScreen();
                cout << "Nom du joueur " << nb << " : ";
                cin >> name;

                cout << endl;
                ColorChoose (color);

                CPairString NameColor = make_pair (name, color);
                VNameColor.push_back (NameColor);

                ++nb;
            }
        }
        return VNameColor;
    } // InitPlayers ()

    /*!
     * \fn void SaveNames (const CVPairStr & VNameColor, const vector<unsigned> & nbWinners, const vector<unsigned> & VScore)
     * \brief Effectue la sauvegarde du nom des gagnants dans un fichier.
     */
    void SaveNames (const CVPairStr & VNameColor, const vector<unsigned> & nbWinners, const vector<unsigned> & VScore){
        ofstream ofs ("Resultats.txt", ios_base::app);
        ofs << "Liste des gagnants :" << endl;

        for (unsigned i(0), j(0); i < VNameColor.size(); i += 2, ++j){
            unsigned score (VScore [nbWinners [j]]);
            ofs << VNameColor [i].first << " vs. " << VNameColor [i + 1].first << " : " << VNameColor [nbWinners [j]].first << " a gagné avec " << score << ((score == 1) ? (" point") : (" points"));
        }
        ofs << endl;
        cout << endl << "Résultats sauvegardés." << endl;
    } // SaveNames ()

    /*!
     * \fn CPosition GeneratePosition (const unsigned & matrixSize, const CPosition & posPlayer1, const CPosition & posPlayer2)
     * \brief Génère des positions différentes de celles des joueurs pour les pions spéciaux.
     */
    CPosition GeneratePosition (const unsigned & matrixSize, const CPosition & posPlayer1, const CPosition & posPlayer2){
        //! Les coordonnées du token sont tirées au hasard dans les bornes de la matrice, et ne peuvent pas valoir les coordonnées des joueurs (grâce aux while).
        unsigned x_token = rand() % matrixSize;
        while (x_token == posPlayer1.first || x_token == posPlayer2.first)
            x_token = rand() % matrixSize;

        unsigned y_token = rand() % matrixSize;
        while (y_token == posPlayer1.second || y_token == posPlayer2.second)
            y_token = rand() % matrixSize;

        CPosition posToken = make_pair (x_token, y_token);
        return posToken;
    } // GeneratePosition ()

    /*!
     * \brief Fait apparaitre le carré rouge aléatoirement.
     */
    void MakeRedSquareAppear (CMatrix         & mat,
                              const char      & KTokenRedSquare,
                              bool            & doesRedSquare,
                              CPosition       & posRedSquare,
                              const unsigned  & chance,
                              const unsigned  & matrixSize,
                              const CPosition & posPlayer1,
                              const CPosition & posPlayer2){
        unsigned random = rand() % 100;
        if (random < chance){
			//! Empêchera la génération d'autres carrés rouges.
            doesRedSquare = false; 
            posRedSquare = GeneratePosition (matrixSize, posPlayer1, posPlayer2);

            //! Le token est introduit dans la matrice.
            mat[posRedSquare.first][posRedSquare.second] = KTokenRedSquare;
        }
    } // MakeRedSquareAppear ()

    /*!
     * \brief Fait apparaitre des pièces aléatoirement.
     */
    void MakeCoinAppear (CMatrix           & mat,
                         const char        & KTokenCoin,
                         vector<CPosition> & VPosCoin,
                         const unsigned    & chance,
                         const unsigned    & matrixSize,
                         const CPosition   & posPlayer1,
                         const CPosition   & posPlayer2){
        unsigned random = rand() % 100;
        if (random < chance){
            CPosition pos = GeneratePosition (matrixSize, posPlayer1, posPlayer2);
            mat[pos.first][pos.second] = KTokenCoin;

            //! Les positions générées sont introduites dans VPosCoin pour pouvoir les comparer avec la position des joueurs plus tard.
            VPosCoin.push_back (pos);
        }
    } // MakeCoinAppear ()
} // namespace

int main (){
    //! 559. Initialisation de la commande rand ().
    srand (time(NULL));

    //! 562. Affichage d'un message de bienvenue.
    ClearScreen();

    Couleur (KBleu);
    for (unsigned i(0); i < 60; ++i)
        cout << '*';
    cout << endl << "*                                                         *" << endl
         << endl << "*               Bienvenue dans le jeu !                   *" << endl
         << endl << "*                                                         *" << endl;
    for (unsigned i(0); i < 60; ++i)
        cout << '*';
    cout << endl << endl;
    Couleur (KReset);

    //! 576. Sélection du mode de jeu.
    unsigned mode;
    cout << "Sélectionner un mode de jeu :" << endl << "1. ";
    Couleur (KBleu);
    cout << "Duel" << endl;
    Couleur (KReset);
    cout << "2. ";
    Couleur (KBleu);
    cout << "Multijoueur" << endl;
    Couleur (KReset);
    ShowPrompt (mode);

    while (mode != 1 && mode != 2){
        cout << endl << "Saisie incorrecte." << endl;
        ShowPrompt (mode);
    }
    Couleur (KReset);
    cout << endl;

    //! 595. Saisie par l'utilisateur de la taille de la matrice (qui est le plateau de jeu).
    unsigned matrixSize;
    cout << "Saisir la ";
    Couleur (KVert);
    cout << "taille ";
    Couleur (KReset);
    cout << "de la matrice :";
    ShowPrompt (matrixSize);

    while (matrixSize <= 1){
        cout << "Veuillez saisir une taille supérieure à 1 : ";
        ShowPrompt (matrixSize);
    }
    Couleur (KReset);

    //! 610. Initialisation de la matrice.
    CMatrix mat;
    InitMat(mat, matrixSize);

    //! 614. On initialise les pseudo/couleur des joueurs.
    CVPairStr VNameColor = InitPlayers (mode);

    //! 617. Initialisation de VScore.
    vector<unsigned> VScore;
    for (unsigned i(0); i < VNameColor.size (); ++i)
        VScore.push_back (0);
	//! 621. Vecteur qui contiendra les "indices" des gagnants dans VNameColor.
    vector<unsigned> nbWinners; 

    //! 624. Boucle principale du jeu. Tourne jusqu'à ce qu'il ne reste plus de couple de joueurs.
    for (unsigned playersCouple (0); playersCouple < VNameColor.size() - 1; playersCouple += 2){

        //! 627. Quand une nouvelle partie commence, on réinitialise la position des joueurs et on fait disparaitre les pions spéciaux.
        CPosition posRedSquare;
        vector<CPosition> VPosCoin;

        CPosition posPlayer1 = make_pair (0, matrixSize - 1);
        CPosition posPlayer2 = make_pair (matrixSize - 1, 0);

        //! 634. On place les pions des joueurs.
        mat[posPlayer1.first][posPlayer1.second] = TokenPlayer1;
        mat[posPlayer2.first][posPlayer2.second] = TokenPlayer2;

        //! 638. (Ré)initialisation de quelques variables pour le jeu.
        bool doesRedSquare = true;
		
        //! 641. Le nombre de tours est proportionnel à la taille de la matrice. Sinon si le plateau est top grand les joueurs ne pourraient pas s'atteindre.
        unsigned compteTour = matrixSize * 1.5;
        unsigned winner(0), cptJ1(0), cptJ2(0);
        char move;

        while (winner == 0){
            if (compteTour == 0) break;

            //! 649. Les instructions se répèteront pour le joueur 2.
            for (unsigned turn(1); turn < 3; ++turn){

                //! 652. Fait apparaitre aléatoirement les pions spéciaux sur le plateau.
                if (doesRedSquare)
                    MakeRedSquareAppear (mat, KTokenRedSquare, doesRedSquare, posRedSquare, 30, matrixSize, posPlayer1, posPlayer2);
                MakeCoinAppear (mat, KTokenCoin, VPosCoin, 30, matrixSize, posPlayer1, posPlayer2);
                ShowMatrix (mat, VNameColor, VScore, playersCouple);

                //! 658. Passage en mode non canonique.
                set_input_mode ();

                //! 661. '1' tour n'est pas pluriel, vérification pour affichage correct.
                Couleur (KBleu);
                cout << compteTour;
                Couleur (KReset);
                cout << ((compteTour != 1) ? (" tours restants") : (" tour restant")) << endl;

                //! 667. Saisie de la valeur du déplacement. Le prompt affiché est le bon pseudo de la bonne couleur.
                cout << "C'est à ";
                if (turn == 1){
                    ShowColoredName (VNameColor[playersCouple]);
                    ++cptJ1;
                }
                else if (turn == 2){
                    ShowColoredName (VNameColor[playersCouple+1]);
                    ++cptJ2;
                }
                cout << "de jouer :" << endl;
                ShowPrompt (move);

                //! 680. Le pion se déplace. C'est le pion 1 qui bouge si on est au tour 1 et inversement.
                MoveToken (mat, move, ((turn == 1) ? (posPlayer1) : (posPlayer2)), turn, VNameColor, playersCouple, matrixSize);
                ShowMatrix (mat, VNameColor, VScore, playersCouple);

                //! 684. Si un des joueurs se trouve sur l'autre, il gagne.
                if ((turn == 1) ? (posPlayer1 == posPlayer2) : (posPlayer2 == posPlayer1)){
                    winner = ((turn == 1) ? (1) : (2));
                    break;
                }
                //! 689. Si un des joueurs se trouve sur le carré rouge, il gagne.
                if ((turn == 1) ? (posPlayer1 == posRedSquare) : (posPlayer2 == posRedSquare)){
                    winner = ((turn == 1) ? (1) : (2));
                    break;
                }
                //! 694. Si un des joueurs se trouve sur une pièce, il gagne un point.
                for (CPosition pos : VPosCoin){
                    if ((turn == 1) ? (posPlayer1 == pos) : (posPlayer2 == pos)){
                        ++VScore [((turn == 1) ? (playersCouple) : (playersCouple + 1))];
                    }
                }
            } // for ()
            --compteTour;
        } // while ()

        if (winner == 0){
            //! 705. Ici la sortie a été induite par "if (CompteTour == 0) break;". Le joueur qui a le plus de points gagne.
            if (VScore [playersCouple] > VScore [playersCouple + 1]){
                ShowColoredName (VNameColor [playersCouple]);
                cout << " gagne grâce à ses " << VScore [playersCouple] << " points." << endl;
            }
            else if (VScore [playersCouple] < VScore [playersCouple + 1]){
                ShowColoredName (VNameColor [playersCouple + 1]);
                cout << " gagne grâce à ses " << VScore [playersCouple + 1] << " points." << endl;
            }
            else if (VScore [playersCouple] == VScore [playersCouple + 1])
                cout << "Match nul !" << endl;

        }
        else if (winner == 1){
            ShowColoredName (VNameColor[playersCouple]);
            cout << "gagne en " << cptJ1 << " tours avec " << VScore [playersCouple] << " points." << endl;

            //! 722. On écrit l'"indice" du joueur 1 de VNameColor dans le vecteur nbWinners.
            nbWinners.push_back (playersCouple);
        }

        else if (winner == 2){
            ShowColoredName (VNameColor[playersCouple+1]);
            cout << "gagne en " << cptJ2 << " tours avec " << VScore[playersCouple + 1] << " points." << endl;

            //! 730. On écrit l'"indice" du joueur 2 de VNameColor dans le vecteur nbWinners.
            nbWinners.push_back (playersCouple+1);
        }

        //! 734. On efface les pions du plateau (ils sont réinitialisés si la boucle for principale refait un tour).
        mat[posPlayer1.first][posPlayer1.second] = KEmpty;
        mat[posPlayer2.first][posPlayer2.second] = KEmpty;

        //! 738. Peu importe ce que l'utilisateur entre, c'est pour mettre le terminal en "pause".
        cout << endl << "Appuyez sur une touche pour continuer";
        cin.get ();

    } // for() principal

    //! 744. Affichage des pseudos des gagnants de la bonne couleur. On affiche les pseudo/couleur des gagnants de VNameColor repérés par les valeurs de nbWinners en indice.
    ClearScreen ();
    cout << ((mode == 1) ? ("Le vainqueur a") : ("Les vainqueurs ont")) << " été :" << endl;
    for (unsigned val : nbWinners){
        Couleur (VNameColor [val].second);
        cout << VNameColor [val].first << endl;
        Couleur (KReset);
    }

    //! 753. On propose à l'utilisateur d'enregistrer le résultat {des, de la} partie(s) dans un fichier Resultat.txt qui est créé s'il n'existe pas.
    char doesSave;
    cout << endl << "Souhaitez-vous enregistrer les résultats des parties ? ('o' ou 'n') ";
    ShowPrompt (doesSave);

    while (doesSave != 'o' && doesSave != 'n'){
        cout << "Saisie incorrecte. Veuillez saisir 'o' ou 'n' : ";
        ShowPrompt (doesSave);
    }

    if (doesSave == 'o')
        SaveNames (VNameColor, nbWinners, VScore);

    //! 766. Retour en mode canonique.
    reset_input_mode ();

    cout << endl;
    return 0;
} // main ()
