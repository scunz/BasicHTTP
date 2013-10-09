/*
 * bHTTP
 *
 * Copyright (C) 2013 Cunz RaD Ltd
 * Copyright (C) 2013 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QString>
#include <QStringList>
#include <QProcess>

#include "libHttpServer/Server.hpp"
#include "libHttpServer/ContentProvider.hpp"

int findPort(const QString& portName) {
    bool bOk = false;

    int portNr = portName.toInt(&bOk);
    if (bOk) {
        return portNr;
    }

    qsrand(QTime::currentTime().msec());
    portNr = 22000 + int( 1000.0 * qrand() / RAND_MAX);

    QFile fPort(portName);
    if (!fPort.open(QFile::WriteOnly)) {
        exit(1);
    }
    fPort.write(QString::number(portNr).toLocal8Bit());
    return portNr;
}

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    if (app.arguments().count() != 3) {
        puts("Usage:\n"
             "\tbHTTP [dir] [port]\n"
             "\n"
             "[dir] is the base directory to serve files from.\n"
             "[Port] is either a fixed port number to listen on or a filename where to put the used"
             "(random) port number into");
        return 1;
    }

    QString dir = app.arguments()[1];

    if (!QDir(dir).exists()) {
        return 1;
    }

    int port = findPort(app.arguments()[2]);
    QString pidText = QString::number(QCoreApplication::applicationPid());
    printf("Running for %s on port %i with pid %s\n", qPrintable(dir), port, qPrintable(pidText));
    fflush(stdout);

    HTTP::Server s;
    HTTP::StaticContentProvider* cp = new HTTP::StaticContentProvider(QString(), dir, &s);
    cp->addMimeType(QRegExp(QLatin1String(".*\\.html$")), "text/html");
    s.addProvider(cp);
    s.listen(port);

    return app.exec();
}
