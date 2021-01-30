#ifndef APPSTYLE_H
#define APPSTYLE_H

#include "app-version.h"

#include <QString>
#include <QStringList>
#include <QFontDatabase>
#include <QFileInfo>
#include <QApplication>
#include <QtDebug>
#include <QRegularExpression>
#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>

class DuUI
{
public:
    DuUI()
    {

    }
    static void updateCSS(QString cssFileName)
    {
        qApp->setStyleSheet("");
        QStringList cssFiles(cssFileName);
        //check if there's a dume file to include
        QFileInfo cssFileInfo(cssFileName);
        QString includeName = cssFileInfo.completeBaseName() + "-" + QString(STR_INTERNALNAME).toLower();
        QString includePath = cssFileInfo.path() + "/" + includeName + ".css";
        QFile includeFile(includePath);
        includePath = cssFileInfo.path() + "/" + includeName;
        if (!includeFile.exists()) includeFile.setFileName(includePath);
        if (includeFile.exists())
        {
            cssFiles << includePath;
        }
        QString css = DuUI::loadCSS(cssFiles);
        qApp->setStyleSheet(css);
    }

    /**
     * @brief loadCSS Loads a CSS File
     * If any cssFileName-values.val file is found, uses these values in the CSS
     * Note: in the values file name, the .val extension can be omitted, or replaced by .txt
     * @param cssFileName The file name (with complete path) of the CSS
     * @return the CSS string
     */
    static QString loadCSS(QString cssFileName = ":/styles/default")
    {
        return loadCSS(QStringList(cssFileName));
    }

    /**
     * @brief loadCSS Loads multiple CSS Files
     * If any cssFileName[0]-values.val file is found, uses these values in the CSS
     * Note: in the values file name, the .val extension can be omitted, or replaced by .txt
     * @param cssFileNames The file names (with complete path) of the CSS
     * @return the CSS string
     */
    static QString loadCSS(QStringList cssFileNames, QString styleValues = "")
    {
        QString css = "";

        //Build a single CSS with the files
        foreach(QString file,cssFileNames)
        {
            QFile cssFile(file);
            if (cssFile.exists())
            {
                if (cssFile.open(QFile::ReadOnly))
                {
                    css += QString(cssFile.readAll());
                    cssFile.close();
                }
            }
        }

        //replace variables
        //find values
        QFileInfo cssInfo(cssFileNames[0]);
        if (styleValues =="")
        {
            styleValues = cssInfo.path() + "/" + cssInfo.completeBaseName() + "-values";
        }
        QFile valuesFile(styleValues);
        if (valuesFile.exists())
        {
            if (valuesFile.open(QFile::ReadOnly))
            {
                css += "\n";
                //read lines
                while(!valuesFile.atEnd())
                {
                    QString line = valuesFile.readLine();

                    //get value and name
                    QRegularExpression re("(@\\S+) *= *(\\S+)");
                    QRegularExpressionMatch match = re.match(line);
                    if (match.hasMatch())
                    {
                        QString name = match.captured(1);
                        QString value = match.captured(2);
                        css.replace(name,value);
                    }
                }
            }
        }

#ifdef QT_DEBUG
        qDebug() << "DuUI: CSS Ready";
#endif

        return css;
    }

    static void setFont(QString family = "Ubuntu", int size=10, int weight=400)
    {
        if (family == "Ubuntu")
        {
            //add fonts
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-B");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-BI");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-C");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-L");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-LI");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-M");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-MI");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-R");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-RI");
            QFontDatabase::addApplicationFont(":/fonts/Ubuntu-Th");
            QFontDatabase::addApplicationFont(":/fonts/UbuntuMono-B");
            QFontDatabase::addApplicationFont(":/fonts/UbuntuMono-BI");
            QFontDatabase::addApplicationFont(":/fonts/UbuntuMono-R");
            QFontDatabase::addApplicationFont(":/fonts/UbuntuMono-RI");
        }

        qApp->setFont(QFont(family,10,500),"QWidget");
    }

    static void setToolButtonStyle(Qt::ToolButtonStyle style)
    {
        foreach( QWidget *w, qApp->allWidgets())
        {
            if(QMainWindow *mw = qobject_cast<QMainWindow*>(w))
            {
                mw->setToolButtonStyle(style);
            }
            else if (QToolBar *tb = qobject_cast<QToolBar*>(w))
            {
                tb->setToolButtonStyle(style);
            }
            else if (QToolButton *tb = qobject_cast<QToolButton*>(w))
            {
                if (tb->objectName() == "windowButton") continue;
                if (tb->text() == "") continue;
                if (tb->icon().isNull()) continue;
                tb->setToolButtonStyle(style);
            }
        }
    }
};


#endif // APPSTYLE_H
