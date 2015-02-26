#include "MetadataWidget.hpp"

#include "Document/ModelMetadata.hpp"
#include "InspectorInterface/InspectorSectionWidget.hpp"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QToolButton>

MetadataWidget::MetadataWidget (ModelMetadata* metadata, QWidget* parent) :
    QWidget (parent),
    m_metadata {metadata}
{
    QVBoxLayout* metadataLayout = new QVBoxLayout{this};
    setLayout (metadataLayout);

    QHBoxLayout* typeLay = new QHBoxLayout{};
    // type
    m_typeLb = new QLabel ("type");

    // LABEL : label + lineEdit in a container
    QWidget* descriptionWidget = new QWidget {this};
    QFormLayout* descriptionLay = new QFormLayout;

    m_scriptingNameLine = new QLineEdit{metadata->name(), this};
    m_labelLine = new QLineEdit{metadata->label(), this};

    descriptionLay->addRow ("Name", m_scriptingNameLine);
    descriptionLay->addRow ("Label", m_labelLine);

    descriptionWidget->setObjectName ("Description");

    descriptionWidget->setLayout (descriptionLay);

    // previous inspector button
    QToolButton* goPrev = new QToolButton{this};
    goPrev->setArrowType (Qt::LeftArrow);

    // color
    m_colorButton = new QPushButton{};
    m_colorButton->setMaximumSize (QSize (1.5 * m_colorIconSize, 1.5 * m_colorIconSize) );
    m_colorButton->setIconSize (QSize (m_colorIconSize, m_colorIconSize) );
    m_colorButtonPixmap.fill (metadata->color() );
    m_colorButton->setIcon (QIcon (m_colorButtonPixmap) );
    typeLay->addWidget (goPrev);
    typeLay->addWidget (m_colorButton);
    typeLay->addWidget (m_typeLb);

    // comments
    m_comments = new QTextEdit{metadata->comment(), this};
    InspectorSectionWidget* comments = new InspectorSectionWidget ("Comments");
    comments->addContent (m_comments);
    comments->expand(); // todo à enlever par la suite

//    metadataLayout->addWidget (m_colorButton);
//    metadataLayout->addWidget(m_typeLb);
    metadataLayout->addLayout (typeLay);
    metadataLayout->addWidget (descriptionWidget);
    metadataLayout->addWidget (comments);

    connect (m_scriptingNameLine, &QLineEdit::editingFinished,
             [ = ] ()
    {
        emit scriptingNameChanged (m_scriptingNameLine->text() );
    });
    connect (m_labelLine, &QLineEdit::editingFinished,
             [ = ] ()
    {
        emit labelChanged (m_labelLine->text() );
    });
    connect (m_comments, &QTextEdit::textChanged,
             [ = ] ()
    {
        emit commentsChanged (m_comments->toPlainText() );
    });
    connect (m_colorButton,  &QPushButton::clicked,
             this,           &MetadataWidget::changeColor);


    connect (metadata,   &ModelMetadata::metadataChanged,
             this,       &MetadataWidget::updateAsked);

    connect (goPrev, &QToolButton::clicked,
             this,   &MetadataWidget::inspectPreviousElement);
}

QString MetadataWidget::scriptingName() const
{
    return m_scriptingNameLine->text();
}

void MetadataWidget::setScriptingName (QString arg)
{
    if (m_scriptingNameLine->text() == arg)
    {
        return;
    }

    m_scriptingNameLine->setText (arg);
    emit scriptingNameChanged (arg);
}

void MetadataWidget::changeColor()
{
    QColor color = QColorDialog::getColor (m_metadata->color(), this, "Select Color");

    if (color.isValid() )
    {
        emit colorChanged (color);
    }
}

void MetadataWidget::setType (QString type)
{
    m_typeLb->setText (type);
}

void MetadataWidget::updateAsked()
{
    m_scriptingNameLine->setText (m_metadata->name() );
    m_labelLine->setText (m_metadata->label() );
    m_comments->setText (m_metadata->comment() );

    m_colorButtonPixmap.fill (m_metadata->color() );
    m_colorButton->setIcon (QIcon (m_colorButtonPixmap) );
    // m_currentColor = newColor;
}
