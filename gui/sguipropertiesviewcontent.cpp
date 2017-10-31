#include <sguipropertiesviewcontent.h>

SGuiPropertiesViewContent::SGuiPropertiesViewContent( QWidget* parent, const char* name )
      : QScrollView( parent, name )
{
   mContent = 0;
   setVScrollBarMode( AlwaysOff );
   setHScrollBarMode( AlwaysOff );
   setFrameStyle( Panel | Sunken );
   setLineWidth( 1 );
   setResizePolicy( Manual );   
}

void SGuiPropertiesViewContent::setContent( QWidget * wid )
{
   if( mContent )
      removeChild( mContent );
   
   mContent = wid;

   if( mContent )
   {
      addChild( mContent );
      calculateSize( );
   }
}
   
void SGuiPropertiesViewContent::calculateSize( )
{
   int fw = lineWidth( ) * 2;
   if( mContent )
   {
      QSize newSize = mContent->minimumSizeHint( );
      
      setVScrollBarMode( AlwaysOff );
      setHScrollBarMode( AlwaysOff );
      setMargins( 0, 0, 0, 0 );
      
      if( width( ) - fw < newSize.width( ) )
      {
         setHScrollBarMode( AlwaysOn );
         
         if( ( height( ) - horizontalScrollBar( )->height( ) - fw )
             < newSize.height( ) )
            setVScrollBarMode( AlwaysOn );
         else
            newSize.setHeight( height( ) - horizontalScrollBar( )->height( ) - 2 );
      }
      else if( height( ) - fw < newSize.height( ) )
      {
         setVScrollBarMode( AlwaysOn );
         
         if( ( width( ) - verticalScrollBar( )->width( ) - fw )
             < newSize.width( ) )
            setHScrollBarMode( AlwaysOn );
         else
            newSize.setWidth( width( ) - verticalScrollBar( )->width( ) - fw );
      }
      else
      {
         newSize.setWidth( width( ) - fw );
         newSize.setHeight( height( ) - fw );
      }
      
      resizeContents( newSize.width( ), newSize.height( ) );
      mContent->resize( newSize );
   }
}
   
void SGuiPropertiesViewContent::resizeEvent( QResizeEvent* /* ev */ )
{
   calculateSize( );
}   

