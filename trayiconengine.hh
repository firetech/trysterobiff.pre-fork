/* {{{

    This file is part of trysterobiff -
      a cross-plattform non-polling IMAP new-mail systray notifier.

    Copyright (C) 2022  Joakim Tufvegren

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

}}} */


#ifndef TRAYICONENGINE_HH
#define TRAYICONENGINE_HH

#include <QIcon>
#include <QIconEngine>

class TrayIconEngine : public QIconEngine {
  public:
    TrayIconEngine(QIcon icon);
    void setIcon(QIcon icon);
    void setUnread(size_t count);
    virtual void virtual_hook(int id, void *data);
    virtual QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);
    virtual void paint (QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
    virtual QIconEngine *clone(void) const;
  private:
    bool show_unread;
    QIcon base_icon;
    size_t unread_count;
};

#endif
