#ifndef S_LINE_EDITS_H
#define S_LINE_EDITS_H

#include <qlineedit.h>

/**
 * Lineedit for float input
 */

class SFloatEdit : public QLineEdit
{
   Q_OBJECT
public:
   enum ValidationOp { OpGreater, OpGreaterEqual,
                       OpLess, OpLessEqual };
   /** Simple constructor */
   SFloatEdit( QWidget* parent, const char* name = 0 );
   /**
    * Sets the validation for the lineedit.
    *
    * If checkLower is true, the value has to be >= the lowerValue.
    *
    * If checkUpper is true, the value has to be <= the upperValue.
    *
    * By default no range check is made.
    */
   void setValidation( bool checkLower, double lowerValue,
                       bool checkUpper, double upperValue );
   /**
    * Sets the validation operators for the lower and upper value.
    *
    * Valid values for lower are OpGreater and OpGreaterEqual,
    * valid values for upper are OpLess and OpLessEqual.
    */
   void setValidationOperator( ValidationOp lower, ValidationOp upper );
   /** Returns true, if the text is a valid float in the valid range */
   bool isDataValid( );
   /** Returns the float value */
   double value( ) const;
   /** Sets the value */
   void setValue( double d, int precision = 5 );
signals:
   /** emitted if the text is changed */
   void dataChanged( );
public slots:
   void slotEditTextChanged( const QString& t );
private:
   bool m_bCheckLower, m_bCheckUpper;
   double m_lowerValue, m_upperValue;
   ValidationOp m_lowerOp, m_upperOp;
};


/**
 * Lineedit for int input
 */

class SIntEdit : public QLineEdit
{
   Q_OBJECT
public:
   /** Simple constructor */
   SIntEdit( QWidget* parent, const char* name = 0 );
   /**
    * Sets the validation for the lineedit.
    *
    * If checkLower is true, the value has to be >= the lowerValue.
    *
    * If checkUpper is true, the value has to be <= the upperValue.
    *
    * By default no range check is made.
    */
   void setValidation( bool checkLower, int lowerValue,
                       bool checkUpper, int upperValue );
   /** Returns true, if the text is a valid integer in the valid range */
   bool isDataValid( );
   /** Returns the integer value */
   int value( ) const;
   /** Sets the value */
   void setValue( int i );
signals:
   /** emitted if the text is changed */
   void dataChanged( );
public slots:
   void slotEditTextChanged( const QString& t );
private:
   bool m_bCheckLower, m_bCheckUpper;
   int m_lowerValue, m_upperValue;
};

#endif
