/****************************************************************************

 This file is part of GLC-Player.
 Copyright (C) 2007-2008 Laurent Ribon (laumaya@users.sourceforge.net)
 Version 2.2.0, packaged on July 2010.

 http://www.glc-player.net

 GLC-Player is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 GLC-Player is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GLC-Player; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*****************************************************************************/


#include <GLC_StructOccurrence>
#include "EditPositionDialog.h"

EditPositionDialog::EditPositionDialog(GLC_StructOccurrence* pSTructOccurrence, QWidget *pParent)
: QDialog(pParent)
, m_pOccurrence(pSTructOccurrence)
, m_Matrix(pSTructOccurrence->structInstance()->relativeMatrix())
{
	setupUi(this);

	// Get Matrix translation
	tx->setValue(m_Matrix.getData()[12]);
	ty->setValue(m_Matrix.getData()[13]);
	tz->setValue(m_Matrix.getData()[14]);

	// Get Matrix Euler angle
	QVector<double> angles= m_Matrix.toEuler();
	rx->setValue(angles[0] / glc::PI * 180.0);
	ry->setValue(angles[1] / glc::PI * 180.0);
	rz->setValue(angles[2] / glc::PI * 180.0);

	connect(tx, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));
	connect(ty, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));
	connect(tz, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));

	connect(rx, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));
	connect(ry, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));
	connect(rz, SIGNAL(valueChanged(double)), this, SLOT(updateMatrix()));

}
EditPositionDialog::~EditPositionDialog()
{

}

void EditPositionDialog::reject()
{
	m_pOccurrence->structInstance()->setMatrix(m_Matrix);
	const int occurrenceCount= m_pOccurrence->structInstance()->numberOfOccurrence();
	QList<GLC_StructOccurrence*> occurrencesList= m_pOccurrence->structInstance()->listOfStructOccurrences();
	for (int i= 0; i < occurrenceCount; ++i)
	{
		occurrencesList[i]->updateChildrenAbsoluteMatrix();
	}
	QDialog::reject();
}

void EditPositionDialog::updateMatrix()
{
	double alphaX= glc::toRadian(rx->value());
	double alphaY= glc::toRadian(ry->value());
	double alphaZ= glc::toRadian(rz->value());

	GLC_Matrix4x4 rotationMatrix(GLC_Matrix4x4().fromEuler(alphaX, alphaY, alphaZ));
	m_pOccurrence->structInstance()->setMatrix(GLC_Matrix4x4(tx->value(), ty->value(), tz->value()) * rotationMatrix);
	const int occurrenceCount= m_pOccurrence->structInstance()->numberOfOccurrence();
	QList<GLC_StructOccurrence*> occurrencesList= m_pOccurrence->structInstance()->listOfStructOccurrences();
	for (int i= 0; i < occurrenceCount; ++i)
	{
		occurrencesList[i]->updateChildrenAbsoluteMatrix();
	}

	emit positionUpdated();
}


