%%
%% Nouvelle version du modele de note technique du DEC
%%

\ProvidesClass{note_technique_2010}[20140116]

%%%%%%%%%%%%%%%%%% Variables utilisees dans style CEA %%%%%%%%%%%%%%%%%%
\RequirePackage{ifthen}
\RequirePackage{setspace}

%% Variables booleennes pour la gestion des options
\newboolean{projet}
\newboolean{rapport}
\newboolean{memo}
\newboolean{modeop}
\newboolean{latin}
\newboolean{anti}
\newboolean{index}
\newboolean{anglais}

\newboolean{annexe}
\newboolean{section_annexe}
\newboolean{image_partie}

% Passage des options
\DeclareOption{projet}{\setboolean{projet}{true}}
\DeclareOption{rapport}{\setboolean{rapport}{true}}
\DeclareOption{modeop}{\setboolean{modeop}{true}}
\DeclareOption{latin}{\setboolean{latin}{true}}
\DeclareOption{index}{\setboolean{index}{true}}
\DeclareOption{memo}{\setboolean{memo}{true}}
\DeclareOption{anti}{\setboolean{anti}{true}}
\DeclareOption{anglais}{\setboolean{anglais}{true}}
% \DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions


\RequirePackage[overload]{textcase}

%% ??
\ifthenelse{\boolean{rapport}}{\setboolean{section_annexe}{false}}{\setboolean{section_annexe}{true}}

% Classe de reference
\ifthenelse{\boolean{rapport}}{
  \LoadClass[a4paper,10pt]{report}
}{
  \LoadClass[a4paper,10pt]{article}
}

% Pour les noms en anglais
\ifthenelse{\boolean{anglais}}{
  \newcommand{\annexenom}{Appendix}
  \newcommand{\appendicesname}{Appendices}
  \newcommand{\appendiceslistname}{List of appendices}
  \newcommand{\sommairename}{Table of contents}
%%  \newcommand{\tableauname}{TABLE}
  \newcommand{\listfigurenom}{List of figures}
  \newcommand{\listtablenom}{List of tables}
  \newcommand{\referencesnom}{References}
}{
  \newcommand{\annexenom}{Annexe}
  \newcommand{\appendicesname}{Annexes}
  \newcommand{\appendiceslistname}{Liste des annexes}
  \newcommand{\sommairename}{Sommaire}
%%  \newcommand{\tableauname}{Tableau}
  \newcommand{\listfigurenom}{Liste des figures}
  \newcommand{\listtablenom}{Liste des tableaux}
  \newcommand{\referencesnom}{R\'ef\'erences}
}

% Pour les annexes (avant les paquets hyperrefs)
\makeatletter
\ifthenelse{\boolean{rapport}}{
  \renewcommand\appendix{\par
    \part*{\appendicesname}
    \addcontentsline{toc}{part}{\appendicesname}
    \setboolean{annexe}{true}
    \setcounter{chapter}{0}
    \setcounter{section}{0}
    \setcounter{subsection}{0}
    \gdef\thechapter{\@Alph\c@chapter}
  }
}{
  \renewcommand\appendix{
    \setboolean{annexe}{true}
    \setcounter{section}{0}
    \setcounter{subsection}{0}
    \gdef\thesection{\@Alph\c@section}
  }
}
\makeatother

%%%%%%%%%%%%%%%%%% Variables utilisees dans style CEA %%%%%%%%%%%%%%%%%%
\input{variables_2010}

%%% Pour augmenter l'ecart entre les caracteres
\RequirePackage{soul}

%%% positionnement H des tableaux et figures
\RequirePackage{float}

%%% Les caracteres accentues
\RequirePackage[T1]{fontenc}
\RequirePackage{textcomp}
\ifthenelse{\boolean{latin}}{
  \RequirePackage[latin9]{inputenc}
}{
  \RequirePackage[utf8]{inputenc}
}

%%% quelques symboles
\RequirePackage{pifont}

%%% Le package pour les mathematiques
\RequirePackage{amsmath}

%%% Les polices
%%% Fonte ARIAL de rigueur
\RequirePackage{helvetCEA}
%%\RequirePackage{times}
%%\RequirePackage{pifont}

%%% Entetes et pieds de pages
\RequirePackage{fancyhdr}

%%% Pour les index
\ifthenelse{\boolean{index}}{
  \RequirePackage{makeidx}
}{}

%%% Reference vers la derniere page
\RequirePackage{lastpage} %% Pour pouvoir faire reference a la derniere page

%%% Inclusion des paquets selon l'utilisation de pdflatex ou latex+dvips
\usepackage{ifpdf}
\ifpdf
\RequirePackage[pdftex]{color,graphicx,thumbpdf}
\RequirePackage[pdftex,pdfstartview=FitH,breaklinks,linktocpage]{hyperref}
% pour avoir un pdf version 1.4 (compatible avec acroread 9.x)
\pdfminorversion=4
\else
\RequirePackage[dvips]{color,graphicx,thumbpdf,pstricks}
\RequirePackage[dvips,pdfstartview=FitH,breaklinks,linktocpage]{hyperref} % Permet de generer les hyperlien
\fi
%%\RequirePackage{couleursCEA}
\definecolor{fondpartiesommaire}{rgb}{0.902,0.902,0.902}
\definecolor{cea_orange}{rgb}{1.,0.7019,0.0627}
\definecolor{cea_orangeclair}{rgb}{1., 1., 0.7294}
\definecolor{cea_vert}{rgb}{0.4392,0.7372,0.1215}
\definecolor{cea_vertclair}{rgb}{0.7294,0.8901,0.6039}
\definecolor{cea_bluecurve}{rgb}{0.902,0.902,0.902}
\definecolor{cea_grisfonce}{rgb}{0.372,0.372,0.372}
\definecolor{cea_gris}{rgb}{0.2941,0.3333,0.3411}
\definecolor{cea_grisclair}{rgb}{0.698,0.698,0.698}
\definecolor{cea_grissection}{rgb}{0.875,0.875,0.875}
\definecolor{cea_grissubsection}{rgb}{0.6,0.6,0.6}


%%% Tableaux
\RequirePackage{array}
\newcolumntype{M}[1]{>{\centering}m{#1}}

%%% Tableaux compliques
\RequirePackage{longtable}
\RequirePackage{multirow}
\RequirePackage{colortbl}% Pour pouvoir mettre le fond de certaines cellules de tableaux en couleurs

%%% Localisation en francais
\ifthenelse{\not\boolean{anglais}}{
\RequirePackage[frenchb]{babel}
}{}

% Set page size and margins
\setlength{\voffset}{0pt}
\setlength{\topmargin}{-1in}
\addtolength{\topmargin}{9mm}
%%\setlength{\voffset}{-1in}
%%\setlength{\topmargin}{15truemm}
\setlength{\headheight}{33.5mm}
\setlength{\headsep}{6mm}
%\setlength{\headsep}{22truemm}
\setlength{\textheight}{225mm}
\setlength{\footskip}{10mm}

\setlength{\hoffset}{0pt}
\setlength{\oddsidemargin}{-1in}
\addtolength{\oddsidemargin}{20truemm}
\setlength{\textwidth}{170truemm}
\setlength{\marginparsep}{0mm}
\setlength{\marginparwidth}{0mm}

\setlength{\parskip}{2truemm}
\setlength{\parindent}{0pt}

%%%%%%%%%%%%%%%%%%%%
\renewcommand{\footrulewidth}{0pt}
\renewcommand{\headrulewidth}{0pt}

%%%%%%%%%%%%%%%%%%%%%

\newlength{\fboxruleIni}
\newlength{\fboxsepIni}
\newlength{\tabcolsepIni}
\newlength{\extrarowheightIni}

%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%% Redefinition des titres %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% les parties
\setboolean{image_partie}{false}
\newcommand{\imagepartie}[1]{
  \setboolean{image_partie}{true}
  \def\Proimagepartie{#1}
}
\makeatletter
\def\@part[#1]#2{
  \ifthenelse{\not\boolean{projet}}{
    \ifodd \thepage \else
    \thispagestyle{fancy}
    \mbox{}
    \newpage
    \fi
  }{}
  \thispagestyle{fancy}
  \ifnum \c@secnumdepth >-2\relax
      \refstepcounter{part}
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}
    \else
      \addcontentsline{toc}{part}{#1}
    \fi
    \vspace*{\stretch{1}}
    {{\color{cea_orange} \hrule}}
    \vspace*{10\p@}
    {\noindent\huge\bfseries\scshape \partname\nobreakspace\thepart}\par\nobreak
    \vspace*{10\p@}
    {{\color{cea_vert} \hrule}}
    \par
    \vspace*{1\p@}
    {{\color{cea_vert} \hrule}}
    \par
    \vskip 20\p@
    {\centering \Huge \bfseries\scshape #2\par}
    \vspace*{7\p@}
    \begin{center}
      \ifthenelse{\boolean{image_partie}}{
        \resizebox{7cm}{!}{\Proimagepartie}
      }{}
    \end{center}
    \vspace*{\stretch{1}}
    \@endpart
    \setboolean{image_partie}{false}
  }
  \def\@spart#1{
  \ifthenelse{\not\boolean{projet}}{
    \ifodd \thepage \else
    \thispagestyle{fancy}
    \mbox{}
    \newpage
    \fi
  }{}
  \thispagestyle{fancy}
  {\centering
    \interlinepenalty \@M
    \normalfont
    \Huge \bfseries\scshape #1\par}
  \@endpart}

\def\@endpart{
  \vfil\newpage
  \if@twoside
  \if@openright
  \null
  \newpage
  \fi
  \fi
  \if@tempswa
  \twocolumn
  \fi
}
\makeatother

%% Les chapitres

\makeatletter

\def\@chapter[#1]#2{
  \ifnum \c@secnumdepth >\m@ne
  \refstepcounter{chapter}
  \typeout{\@chapapp\space\thechapter.}
  \ifthenelse{\not\boolean{annexe}}{
    \addcontentsline{toc}{chapter}{\protect\numberline{\thechapter}#1}
  }{
    \ifthenelse{\boolean{anti}}{%
    \addcontentsline{toc}{chapter}{\annexenom{} \protect\numberline{\thechapter}#1}}{%
    \addcontentsline{toa}{chapter}{\annexenom{} \protect\numberline{\thechapter}#1}}
  }
  \else
  \addcontentsline{toc}{chapter}{#1}
  \fi
  \chaptermark{#1}
  \addtocontents{lof}{\protect\addvspace{10\p@}}
  \addtocontents{lot}{\protect\addvspace{10\p@}}
  \if@twocolumn
  \@topnewpage[\@makechapterhead{#2}]
  \else
  \@makechapterhead{#2}
  \@afterheading
  \fi
}

\newlength{\longueurchap}
\setlength{\longueurchap}{\textwidth}
\addtolength{\longueurchap}{10.2truemm}
\def\@makechapterhead#1{
  \ifthenelse{\not\boolean{projet}}{
    \ifodd \thepage \else
    \thispagestyle{fancy}
    \mbox{}
    \newpage
    \fi
  }{}
  \hskip-0.8cm
  \thispagestyle{fancy}
  \colorbox{cea_grissection}{
    \begin{minipage}[htbp]{\longueurchap}
      \ifthenelse{\not\boolean{annexe}}{
        {\Huge \bfseries\scshape \@chapapp{} \thechapter~: #1}
        \par
      }{
        {\Huge \bfseries\scshape \annexenom{} \thechapter~: #1}
        \par
      }
    \end{minipage}
  }
  \vskip 30pt
}

\def\@schapter#1{
  \if@twocolumn
  \@topnewpage[\@makeschapterhead{#1}]
  \else
  \@makeschapterhead{#1}
  \@afterheading
  \fi}

\def\@makeschapterhead#1{
  \ifthenelse{\not\boolean{projet}}{
    \ifodd \thepage \else
    \thispagestyle{fancy}
    \mbox{}
    \newpage
    \fi
  }{}
  \hskip-0.8cm
  \thispagestyle{fancy}
  \colorbox{cea_grissection}{
    \begin{minipage}[htbp]{\longueurchap}
      {\Huge \bfseries\scshape #1}
      \par
    \end{minipage}
  }
  \vskip 30pt
}

\makeatother

\makeatletter
\def\@startsectioncea#1#2#3#4#5#6{%
  \if@noskipsec \leavevmode \fi
  \par
  \@tempskipa #4\relax
  \@afterindenttrue
  \ifdim \@tempskipa <\z@
    \@tempskipa -\@tempskipa \@afterindentfalse
  \fi
  \if@nobreak
    \everypar{}%
  \else
    \addpenalty\@secpenalty\addvspace\@tempskipa
  \fi
  \@ifstar
    {\@ssect{#3}{#4}{#5}{#6}}%
    {\@dblarg{\@sectcea{#1}{#2}{#3}{#4}{#5}{#6}}}}
%
\def\@sectcea#1#2#3#4#5#6[#7]#8{%
  \ifnum #2>\c@secnumdepth
    \let\@svsec\@empty
  \else
    \refstepcounter{#1}%
    \protected@edef\@svsec{\@seccntformat{#1}\relax}%
  \fi
  \@tempskipa #5\relax
  \ifdim \@tempskipa>\z@
    \begingroup
      #6{%
        \ifthenelse{\boolean{section_annexe}\and\boolean{annexe}}{%
         \@hangfrom{\hskip #3\relax\annexenom~\@svsec}%
           \interlinepenalty \@M #8\@@par%
        }{%
         \@hangfrom{\hskip #3\relax\@svsec}%
           \interlinepenalty \@M #8\@@par}%
        }
    \endgroup
    \csname #1mark\endcsname{#7}%
    \ifthenelse{\boolean{section_annexe}\and\boolean{annexe}}{%
    \ifthenelse{\boolean{anti}}{%
      \addcontentsline{toc}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect{\annexenom} \numberline{\csname the#1\endcsname}%
        \fi
      #7}}{%
%      \addcontentsline{toa}{annexe} accoladeouverte%
      \addcontentsline{toa}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect{\annexenom} \numberline{\csname the#1\endcsname}%
        \fi
      #7}}%
    }{%
      \addcontentsline{toc}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect\numberline{\csname the#1\endcsname}%
        \fi
      #7}%
    }
  \else
    \def\@svsechd{%
      #6{\hskip #3\relax
      \@svsec #8}%
      \csname #1mark\endcsname{#7}%
      \addcontentsline{toc}{#1}{%
        \ifnum #2>\c@secnumdepth \else
          \protect\numberline{\csname the#1\endcsname}%
        \fi
        #7}}%
  \fi
  \@xsect{#5}}
\makeatother


%% Redefinition de la section
%%----------------------------
\newlength{\longueursection}
\setlength{\longueursection}{170mm}
\addtolength{\longueursection}{4pt}

\newcommand{\miseenpagesection}[1]{
  % Redefinir l'ecartement entre les lignes
  \let\tmpcmd\baselinestretch%
  \renewcommand{\baselinestretch}{1}%
  \ifthenelse{\boolean{projet}}{%
      \hbox to \hsize{\vbox{\parindent=0pt\Large\bfseries\scshape\raggedright #1\par}}%
  }{%
    \hskip -4pt%
    \setlength{\fboxsepIni}{\the\fboxsep}%
    \setlength{\fboxsep}{2pt}%
    \colorbox{cea_grissection}{\rule{0mm}{4.5mm}% une hauteur minimum (pour les accents sur les majuscules)
      \hsize=\longueursection%
      \hbox to \hsize{\vbox{\parindent=40pt\Large\bfseries\scshape #1}}%
    }%
    \setlength{\fboxsep}{\the\fboxsepIni}%
  }%
  \vskip 5mm \nobreak%
  % Reprendre l'ecartement entre les lignes
  \let\baselinestretch\tmpcmd%
}

\makeatletter
\renewcommand{\section}{\@startsectioncea{section}{1}{0mm}%
  {-3.5ex \@plus -1ex \@minus -.2ex}%
  {2ex \@plus .2ex}%
  {\miseenpagesection}}%
\makeatother

%% Redefinition de la subsection
%%-------------------------------
%\newlength{\longueursubsection}
%\setlength{\longueursubsection}{\longueursection}
%\addtolength{\longueursubsection}{-5.8truemm}

\newcommand{\miseenpagesubsection}[1]{%
  % Redefinir l'ecartement entre les lignes
  \let\tmpcmd\baselinestretch%
  \renewcommand{\baselinestretch}{1}%
  \hbox to \hsize{\vbox{\parindent=0pt\large\scshape #1\par%
      \vskip -2ex%
      \ifthenelse{\boolean{projet}}{%
        \rule{0mm}{0.1pt}%
      }{%
        \color{cea_grissubsection}\rule{\textwidth}{0.5pt}%
      }%
    }%
  }%
%  \vskip 2mm \nobreak%
  % Reprendre l'ecartement entre les lignes
  \let\baselinestretch\tmpcmd%
}

\makeatletter
\renewcommand{\subsection}{\@startsectioncea{subsection}{2}{0mm}%
  {-3.25ex\@plus -1ex \@minus -.2ex}%
  {1.7ex \@plus .2ex}%
  {\miseenpagesubsection}}
\makeatother

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% References  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\referencecea}
{
  \noindent
  \ifthenelse{\boolean{rapport}}{
    \refstepcounter{chapter}% pour les liens hypertexte
    \addcontentsline{toc}{chapter}{\referencesnom}
  }{
    \refstepcounter{section}% pour les liens hypertexte
    \addcontentsline{toc}{section}{\referencesnom}
  }
  \rule{0pt}{0pt}
  \begin{center}
    {\Large\bf\scshape\so\referencesnom}
  \end{center}
  \vspace{5truemm}
  \ifthenelse{\isundefined{\Prostylebib}}{}{\bibliographystyle{\Prostylebib}}
  \ifthenelse{\isundefined{\Profichierbib}}{}{\bibliography{\Profichierbib}}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Index  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\makeatletter
\ifthenelse{\boolean{index}}{
  \renewcommand\printindex{
    \ifthenelse{\boolean{rapport}}{
      \refstepcounter{chapter}% pour les liens hypertexte
      \addcontentsline{toc}{chapter}{Index}
    }{
      \refstepcounter{section}% pour les liens hypertexte
      \addcontentsline{toc}{section}{Index}
    }
    \@input@{\jobname.ind}
  }

  \renewenvironment{theindex}{
    \if@twocolumn
    \@restonecolfalse
    \else
    \@restonecoltrue
    \fi
    \columnseprule \z@
    \columnsep 35\p@
    \ifthenelse{\boolean{rapport}}{
      \twocolumn[\@makeschapterhead{\indexname}]
    }{
      \twocolumn[\section*{\indexname}]
    }
    \@mkboth{\MakeUppercase\indexname}
    {\MakeUppercase\indexname}
    \parindent\z@
    \parskip\z@ \@plus .3\p@\relax
    \let\item\@idxitem
  }
  {\if@restonecol\onecolumn\else\clearpage\fi}
}{}

\makeatother


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Liste des figures   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\makeatletter
\newcommand{\listefigures}
{
  \clearpage
  \ifthenelse{\boolean{rapport}}{
    \refstepcounter{chapter}% pour les liens hypertexte
    \addcontentsline{toc}{chapter}{\listfigurenom}
  }{
    \refstepcounter{section}% pour les liens hypertexte
    \addcontentsline{toc}{section}{\listfigurenom}
  }
  \rule{0pt}{0pt}
  \begin{center}
    {\Large\bf\scshape\so\listfigurenom}
  \end{center}
  \vspace{5truemm}
  %\listoffigures
  \@starttoc{lof}
  \newpage
}
\makeatother


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Liste des tableaux  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\makeatletter
\newcommand{\listetableaux}
{
  \clearpage
  \ifthenelse{\boolean{rapport}}{
    \refstepcounter{chapter}% pour les liens hypertexte
    \addcontentsline{toc}{chapter}{\listtablenom}
  }{
    \refstepcounter{section}% pour les liens hypertexte
    \addcontentsline{toc}{section}{\listtablenom}
  }
  \rule{0pt}{0pt}
  \begin{center}
    {\Large\bf\scshape\so\listtablenom}
  \end{center}
  \vspace{5truemm}
  \@starttoc{lot}
  \newpage
}
\makeatother

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Pieds de page                  %%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\makeatletter
\renewcommand\@makefnmark{\@textsuperscript{{\tiny\@thefnmark}}}
\makeatother


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Formatage des figures          %%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Provisoire : dans la version 2 de frenchb, c'est par defaut !!!
\makeatletter
%\renewcommand{\fnum@figure}{\scshape \figurename\nobreakspace\thefigure}
%\renewcommand{\fnum@table}{\scshape \tableauname\nobreakspace\thetable}
\ifthenelse{\not\boolean{anglais}}{
\addto\captionsfrench{\def\figurename{{\scshape Figure}}}
\addto\captionsfrench{\def\tablename{{\scshape Tableau}}}
}
\makeatother

% Mise en page des figures et des tableaux

\makeatletter
\newsavebox{\tempbox}
\renewcommand{\@makecaption}[2]
{
  %%\vspace{-5pt}
 \ifthenelse{\boolean{anti}}
 {\sbox{\tempbox}{\underline{\bf #1~:} #2}
  \ifdim \wd\tempbox >\hsize
    \underline{\bf #1~:} #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\tempbox\hfil}%
  \fi}
 {\sbox{\tempbox}{\scshape \underline{\bf #1~:} #2}
  \ifdim \wd\tempbox >\hsize
    \scshape \underline{\bf #1~:} #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\tempbox\hfil}%
  \fi}
  %%\vspace{5mm}
}
% Pour les 'longtable'
\def\LT@makecaption#1#2#3{
  \LT@mcol\LT@cols c{\hbox to\z@{\hss\parbox[c]\textwidth{\medskip
       \ifthenelse{\boolean{anti}}
       {\sbox\@tempboxa{\normalsize\underline{\bf #1{#2~:}}{ }#3}
        \ifdim\wd\@tempboxa>\hsize
          \normalsize\underline{\bf #1{#2~:}}{ }#3
        \else  \hbox to\hsize{\hfil\box\@tempboxa\hfil}
        \fi}
       {\sbox\@tempboxa{\normalsize\scshape \underline{\bf #1{#2~:}}{ }#3}
        \ifdim\wd\@tempboxa>\hsize
          \normalsize\scshape \underline{\bf #1{#2~:}}{ }#3
        \else  \hbox to\hsize{\hfil\box\@tempboxa\hfil}
        \fi}
       \endgraf\medskip}
      \hss}}}

\makeatother


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%% Le sommaire %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


\makeatletter

\ifthenelse{\boolean{rapport}}{
  \setcounter{secnumdepth}{3}
  \setcounter{tocdepth}{1}
}{}

\makeatletter
\def\tableofcontents{
  \newlength{\@parskiptmptableofcontents}
  \setlength{\@parskiptmptableofcontents}{\parskip}
  \parskip 2mm
  \begin{center}
    {\Large\bf\scshape\so\sommairename}
  \end{center}
  \@starttoc{toc}}
\makeatother

\makeatletter
\ifthenelse{\boolean{rapport}}{

  \renewcommand\@dotsep{1.}
  \definecolor{fondpartiesommaire}{rgb}{0.902,0.902,0.902}
  \renewcommand*\l@part[2]{
    \vskip 1.em
    \begingroup
    \noindent
    \hskip-0.5cm
    \fcolorbox{black}{fondpartiesommaire}{
      \begin{minipage}[h]{1.01\textwidth}
        \leavevmode {\bfseries \large \scshape #1}\nobreak
        % \hfill
        \dotfill
        {\bfseries #2}
      \end{minipage}
    }\nobreak
    \endgroup\par
  }

  \renewcommand*\l@chapter[2]{
    \ifnum \c@tocdepth >\m@ne
    \addpenalty{-\@highpenalty}
    \vskip 1.0em \@plus\p@
    \setlength\@tempdima{1.5em}
    \hskip-0.4cm
    \begingroup
    \parindent \z@ \rightskip \@pnumwidth
    \parfillskip -\@pnumwidth
    \leavevmode
    \advance\leftskip\@tempdima
    \hskip -\leftskip
    \bfseries {\large \scshape #1}\nobreak\dotfill \nobreak\hb@xt@\@pnumwidth{\hss  #2}\par
    \penalty\@highpenalty
    \endgroup
    \fi}

  \renewcommand*\l@section[2]{%
    \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    % \addvspace{1.0em \@plus\p@}%
    \setlength\@tempdima{1.5em}%
    \begingroup
    \parindent \z@ \rightskip \@pnumwidth
    \parfillskip -\@pnumwidth
    \leavevmode {
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      \bfseries \scshape #1}\nobreak\
    % {\bfseries \dotfill}
    \dotfill
    \hfil \nobreak\hb@xt@\@pnumwidth{\hss \bfseries #2}\par
    \endgroup
    \fi}

  \newlength\@retraitsubsection
  \setlength\@retraitsubsection{\leftskip}
  \renewcommand*\l@subsection[2]{
    \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    % \addvspace{1.0em \@plus\p@}
    \ifthenelse{\boolean{rapport}}{
      \setlength\@tempdima{3.em}
    }{
      \setlength\@tempdima{2.5em}
    }
    \begingroup
    \parindent \z@ \rightskip \@pnumwidth
    \parfillskip -\@pnumwidth
    \leavevmode {
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      \hskip 2em
      \scshape #1}\nobreak\
    \dotfill
    \hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
    \endgroup
    \fi}

}{

  \makeatletter

 %% ce code provient de la commande \@dottedtocline du fichier latex.ltx
\def\newdottedtocline#1#2#3#4#5#6{%
  \ifnum #1>\c@tocdepth \else
    \vskip \z@ \@plus.2\p@
    {\leftskip #2\relax \rightskip \@tocrmarg \parfillskip -\rightskip%
     \parindent #2\relax\@afterindenttrue%
     \interlinepenalty\@M%
     \leavevmode%
     \@tempdima #3\relax%
     \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip\hskip #2 \hskip -3.5pt%
   {#4 #5}\nobreak%
     \leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{#4 .}\mkern \@dotsep
        mu$}\hfill%
     \nobreak%
     \hb@xt@\@pnumwidth{\hfil\normalfont \normalcolor #4 #6}%
     \par}%
  \fi}

 \renewcommand*\l@section{\newdottedtocline{1}{0em}{1.5em}{\bfseries \scshape}}

  \newlength\@retraitsubsection
  \setlength\@retraitsubsection{0pt}
  \renewcommand*\l@subsection{\newdottedtocline{2}{2em}{2.7em}{\scshape}}
  \renewcommand*\l@subsubsection{\newdottedtocline{3}{4.5em}{3.4em}{\it}}

  \renewcommand*\l@paragraph{\newdottedtocline{4}{7.0em}{4.3em}{ }}

    \makeatother
}

\makeatletter
\renewcommand{\@dotsep}{0.4}
%% ce code provient de la commande \@dottedtocline du fichier latex.ltx
%% si on change de police, il faut ajuster les decallages 3.8em et 1.7em
\renewcommand*\l@figure[2]{
  \ifnum 1>\c@tocdepth \else
    \vskip \z@ \@plus.2\p@
    {\leftskip 3.8em\relax \rightskip \@tocrmarg \parfillskip -\rightskip
     \parindent 3.8em\relax\@afterindenttrue
     \interlinepenalty\@M
     \leavevmode
     \@tempdima 1.7em\relax
     \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
     {{\scshape\figurename}\nobreakspace%
      \ifthenelse{\boolean{anti}}
       {#1}
       {\scshape#1}
     }\nobreak
     \mbox{}\leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep
        mu$}\hfill
     \nobreak
     \hb@xt@\@pnumwidth{\hfil\normalfont \normalcolor #2}%
     \par}%
  \fi}
%% ce code provient de la commande \@dottedtocline du fichier latex.ltx
%% si on change de police, il faut ajuster les decallages 4.3em et 1.7em
\renewcommand*\l@table[2]{
  \ifnum 1>\c@tocdepth \else
    \vskip \z@ \@plus.2\p@
    {\leftskip 4.3em\relax \rightskip \@tocrmarg \parfillskip -\rightskip
     \parindent 4.3em\relax\@afterindenttrue
     \interlinepenalty\@M
     \leavevmode
     \@tempdima 1.7em\relax
     \advance\leftskip \@tempdima \null\nobreak\hskip -\leftskip
     {{\scshape\tablename}\nobreakspace%
      \ifthenelse{\boolean{anti}}
       {#1}
       {\scshape#1}
     }\nobreak
     \mbox{}\leaders\hbox{$\m@th
        \mkern \@dotsep mu\hbox{.}\mkern \@dotsep
        mu$}\hfill
     \nobreak
     \hb@xt@\@pnumwidth{\hfil\normalfont \normalcolor #2}%
     \par}%
  \fi}

  \newcommand*\l@annexe[2] {%
    \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    \ifthenelse{\boolean{anti}}{%
      \addvspace{1.0em \@plus\p@}%
      \setlength\@tempdima{1.5em}%
    }{%
      %\addvspace{1.0em \@plus\p@}%
      \setlength\@tempdima{1.5em}%
    }
    \begingroup
    \parindent \z@ \rightskip \@pnumwidth
    \parfillskip -\@pnumwidth
    \leavevmode
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      % Decalage vraiment pas beau ;- <- Castel
      \scshape\bfseries\large
%      \ifthenelse{\boolean{anti}}{\bfseries}{\large}%
      #1\nobreak
    \dotfill
    \hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
    \endgroup
    \fi
  }
\makeatother


%%% definition des references : copie du fichier article.cls
%%% avec suppression de la section !

\makeatletter
\renewenvironment{thebibliography}[1]
     {%\section*{\refname}
      \@mkboth{\MakeUppercase\refname}{\MakeUppercase\refname}
      \list{\@biblabel{\@arabic\c@enumiv}}
           {\settowidth\labelwidth{\@biblabel{#1}}
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}
      \endlist}
\makeatother

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Gestion des annexes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Liste des annexes
\makeatletter
\newcommand{\listeannexes}{
\clearpage
\ifthenelse{\boolean{anti}}{%
  \ClassWarning {note_technique_2010}{Commande listeannexes incompatible avec l'option anti : on l'ignore}
  %\ClassError{note_technique_2010}{Commande listeannexes incompatible avec l'option anti}{Vous devez supprimer la commande listeannexes avec l'option anti}%
}{%
  \ifthenelse{\boolean{rapport}}{
    \refstepcounter{chapter}% pour les liens hypertexte
    \addcontentsline{toc}{chapter}{\appendiceslistname}
  }{
    \refstepcounter{section}% pour les liens hypertexte
    \addcontentsline{toc}{section}{\appendiceslistname}
  }
  \rule{0pt}{0pt}
  \begin{center}
    {\Large\bf\scshape\so\appendiceslistname}
  \end{center}
  \vspace{5truemm}
  \@starttoc{toa}
  \newpage
}
}
\makeatother

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%% Debut et fin de document       %%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\ifthenelse{\boolean{projet}}
%% Version projet
{
  \AtBeginDocument{
%% Page de Titre et de resume
    \thispagestyle{empty}
    \begin{center}
      \ifthenelse{\isundefined{\Protitre}}{}{\Large\bf \Protitre\\}
      \ifthenelse{\isundefined{\Proauteurs}}{}{\large\bf \Proauteurs\\}
      \ifthenelse{\isundefined{\Prodate}}{}{\large\bf \Prodate}
    \end{center}
    \ifthenelse{\isundefined{\Proaffiliation}}{}{\Proaffiliation}

    \bigskip

    \ifthenelse{\isundefined{\Proresumecea}}{}{
      \section*{R\'esum\'e}
      \Proresumecea
      \newpage

    }
%% Sommaire
    \tableofcontents
    \newpage

  }
}
%% Document definitif
{
  \input{mise_en_page_2010}
  \AtBeginDocument{
    \pagestyle{fancy}
    \PageTitre
    \PageInfo
    \PageResume
    \PageDiffusion
    \tableofcontents
    \newpage
    \onehalfspacing
    % Probleme lie a l'utilisation du style
    % longtable pour les listes de diffusion
    \setcounter{table}{0}
  }
}
