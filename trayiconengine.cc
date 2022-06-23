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


#include "trayiconengine.hh"

#include <QSettings>
#include <QPainter>


TrayIconEngine::TrayIconEngine(QIcon icon)
  : unread_count(0)
{
  QSettings s;
  base_icon = icon;
  show_unread = s.value("gui/show_unread", QVariant(true)).toBool();
}

void TrayIconEngine::setIcon(QIcon icon)
{
  base_icon = icon;
}

void TrayIconEngine::setUnread(size_t count)
{
  unread_count = count;
}

void TrayIconEngine::virtual_hook(int id, void *data)
{
  switch (id) {
  case QIconEngine::AvailableSizesHook: {
    QIconEngine::AvailableSizesArgument &arg =
        *reinterpret_cast<QIconEngine::AvailableSizesArgument*>(data);
    arg.sizes = base_icon.availableSizes(arg.mode, arg.state);
    if (arg.sizes.isEmpty()) {
      // SVG files return an empty list, which causes an empty icon,
      // just return an arbitrary size in this case.
      arg.sizes.append(QSize(64, 64));
    }
    break;
  }
  default:
    QIconEngine::virtual_hook(id, data);
    break;
  }
}

QPixmap TrayIconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
{
  QPixmap pix(size);
  pix.fill(Qt::transparent);
  QPainter painter (&pix);
  QRect r(QPoint(0.0,0.0), size);
  this->paint(&painter, r, mode, state);
  return pix;
}

void TrayIconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
  painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
  base_icon.paint(painter, rect, Qt::AlignCenter, mode, state);
  if (show_unread && unread_count) {
    QFont font = painter->font();
    font.setWeight(QFont::Bold);
    font.setPixelSize(rect.height() >> 1);
    painter->setFont(font);
    Qt::Alignment flags = Qt::AlignHCenter | Qt::AlignVCenter;
    painter->drawText(rect, flags, QString::number(unread_count));
  }
}

QIconEngine *TrayIconEngine::clone(void) const
{
  TrayIconEngine *new_engine = new TrayIconEngine(base_icon);
  new_engine->show_unread = show_unread;
  new_engine->unread_count = unread_count;
  return new_engine;
}
