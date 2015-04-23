/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES RECIPIENT'S ACCEPTANCE
 * OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or
 * http://www.openmodelica.org, and in the OpenModelica distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */


#ifndef TLMEDITOR_H
#define TLMEDITOR_H

#include <QSyntaxHighlighter>

#include "MainWindow.h"
#include "Helper.h"
#include "Utilities.h"
#include "BaseEditor.h"

class MainWindow;
class ModelWidget;

class TLMTabSettings
{
public:
  enum TabPolicy {
    SpacesOnlyTabPolicy = 0,
    TabsOnlyTabPolicy = 1
  };
  TLMTabSettings();
  void setTabPolicy(int tabPolicy) {mTabPolicy = (TabPolicy)tabPolicy;}
  TabPolicy getTabPolicy() const {return mTabPolicy;}
  void setTabSize(int tabSize) {mTabSize = tabSize;}
  int getTabSize() const {return mTabSize;}
  void setIndentSize(int indentSize) {mIndentSize = indentSize;}
  int getIndentSize() {return mIndentSize;}

  int lineIndentPosition(const QString &text) const;
  int columnAt(const QString &text, int position) const;
  int indentedColumn(int column, bool doIndent = true) const;
  QString indentationString(int startColumn, int targetColumn) const;

  static int firstNonSpace(const QString &text);
  static int spacesLeftFromPosition(const QString &text, int position);
private:
  TabPolicy mTabPolicy;
  int mTabSize;
  int mIndentSize;
};

class TLMEditor : public BaseEditor
{
  Q_OBJECT
public:
  TLMEditor(ModelWidget *pModelWidget);
  bool TLMEditorFocusChanged();
signals:
  bool focusOut();
private slots:
  virtual void showContextMenu(QPoint point);
public slots:
  virtual void contentsHasChanged(int position, int charsRemoved, int charsAdded);
  virtual void toggleCommentSelection() {}
};

/*!
  \class MessageHandler
  \brief Defines the appropriate error message of the parsed XML validated againast the XML Schema.
         The class implementation and logic is inspired from Qt Creator sources.
  */
class MessageHandler : public QAbstractMessageHandler
{
public:
  MessageHandler()
      : QAbstractMessageHandler(0)
  {
  }
  QString statusMessage() const { return m_description;}
  int line() const { return m_sourceLocation.line();}
  int column() const { return m_sourceLocation.column();}
protected:
  virtual void handleMessage(QtMsgType type, const QString &description,
                             const QUrl &identifier, const QSourceLocation &sourceLocation)
  {
    Q_UNUSED(type);
    Q_UNUSED(identifier);
    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
  }
private:
  QtMsgType m_messageType;
  QString m_description;
  QSourceLocation m_sourceLocation;
};

class TLMEditorPage;
class TLMHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  TLMHighlighter(TLMEditorPage *pTLMEditorPage, QPlainTextEdit *pPlainTextEdit = 0);
  void initializeSettings();
  void highlightMultiLine(const QString &text);
protected:
  virtual void highlightBlock(const QString &text);
private:
  TLMEditorPage *mpTLMEditorPage;
  QPlainTextEdit *mpPlainTextEdit;
  struct HighlightingRule
  {
    QRegExp mPattern;
    QTextCharFormat mFormat;
  };
  QVector<HighlightingRule> mHighlightingRules;
  QRegExp mCommentStartExpression;
  QRegExp mCommentEndExpression;
  QRegExp mStringStartExpression;
  QRegExp mStringEndExpression;
  QTextCharFormat mTextFormat;
  QTextCharFormat mTagFormat;
  QTextCharFormat mElementFormat;
  QTextCharFormat mQuotationFormat;
  QTextCharFormat mCommentFormat;
public slots:
  void settingsChanged();
};
#endif // TLMEDITOR_H
