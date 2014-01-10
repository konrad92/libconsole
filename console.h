#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <iostream>

#ifdef BUILD_DLL
    #define DECLDLL __declspec(dllexport)
#else
    #define DECLDLL __declspec(dllimport)
#endif

namespace con
{
    /** \brief Utility function for system time (ms) retrieving.
     *
     * \return unsigned int The current system time (ms).
     *
     */
    unsigned int DECLDLL ticks();

    /** \brief Modifier base class.
     */
    class DECLDLL modifier
    {
    public:
        /** \brief Pass the modifier.
         *
         * \param out The destination stream.
         *
         */
        virtual void pass( std::ostream &out ) const = 0;
    };

    /** \brief Console modifier that manages the console window and buffer.
     */
    class DECLDLL dimm : public modifier
    {
    public:
        /** \brief The default constructor.
         *
         * \param width,height Size in characters.
         * \param hidecur Hide the console cursor.
         *
         */
        dimm( unsigned short width, unsigned short height, bool hidecur = false )
            : m_width( width ), m_height( height ), m_hideCursor( hidecur )
        {
            // DUMMY BODY
        }

        /** \brief Pass the modifier.
         *
         * \param out The destination stream.
         *
         */
        void pass( std::ostream &out ) const;

    private:
        unsigned short m_width, m_height;
        bool m_hideCursor;
    };

    /** \brief Console modifier for color changing.
     */
    class DECLDLL color : public modifier
    {
    public:
        /** \brief Colour constants.
         */
        enum
        {
            black     = 0x00,
            blue      = 0x01,
            green     = 0x02,
            red       = 0x04,
            intensity = 0x08,

            white = 0x0F,
            grey  = 0x07,

            yellow = 0x06,
            purple = 0x05,
            cyan   = 0x03,
        };

        /** \brief The default constructor.
         *
         * \param text Foreground text colour.
         * \param bg Background overtext colour.
         *
         */
        color( unsigned char text, unsigned char bg = 0 )
            : m_color( (unsigned short)(((bg&0xF)<<4)|(text&0xF)) )
        {
            // DUMMY BODY
        }

        /** \brief Pass the modifier.
         *
         * \param out The destination stream.
         *
         */
        void pass( std::ostream &out ) const;

    private:
        unsigned short m_color;
    };

    /** \brief Console modifier for cursor position changing.
     */
    class DECLDLL cursor : public modifier
    {
    public:
        /** \brief The default constructor.
         *
         * \param x,y New absolute cursor position.
         *
         */
        cursor( unsigned short x, unsigned short y )
            : m_x( x ), m_y( y )
        {
            // DUMMY BODY
        }

        /** \brief Reset the cursor position (0,0).
         *
         */
        inline void reset()
        {
            m_x = 0;
            m_y = 0;
        }

        /** \brief Set cursor position by given new one.
         *
         * \param x,y Absolute new position.
         *
         */
        inline void set( unsigned short x, unsigned short y )
        {
            m_x = x;
            m_y = y;
        }

        /** \brief Move cursor position by given new one.
         *
         * \param x,y Relative position to the current.
         *
         */
        inline void move( unsigned short x, unsigned short y )
        {
            m_x += x;
            m_y += y;
        }

        /** \brief Retrieve cursor X position.
         *
         * \return unsigned short The current X position.
         *
         */
        inline unsigned short getx() const
        {
            return m_x;
        }

        /** \brief Retrieve cursor X position.
         *
         * \return unsigned short The current X position.
         *
         */
        inline unsigned short gety() const
        {
            return m_y;
        }

        /** \brief Overloaded compare operator.
         *
         * \param cur const cursor&
         * \return bool operator
         *
         */
        bool operator ==( const cursor &cur )
        {
            return (m_x == cur.getx() && m_y == cur.gety());
        }

        /** \brief Pass the modifier.
         *
         * \param out The destination stream.
         *
         */
        void pass( std::ostream &out ) const;

    private:
        unsigned short m_x, m_y;
    };

    /** \brief Console modifier for screen filling.
     */
    class DECLDLL cls : public modifier
    {
    public:
        /** \brief The default constructor.
         *
         * \param fillby Which character use for console buffer filling.
         * \param attr Buffer elements attribute to reset (i.e. colour).
         *
         */
        cls( unsigned char fillby = ' ', unsigned short attr = 0x07 )
            : m_fillBy( fillby ), m_attr( attr )
        {
            // DUMMY BODY
        }

        /** \brief Pass the modifier.
         *
         * \param out The destination stream.
         *
         */
        void pass( std::ostream &out ) const;

    private:
        unsigned char m_fillBy;
        unsigned short m_attr;
    };

    /** \brief Keyboard interface.
     */
    class DECLDLL kb
    {
    public:
        /** \brief Virtual key code constants.
         *         For more VKs, check MSDN.
         */
        enum
        {
            vk_return  = 0x0D,
            vk_shift   = 0x10,
            vk_control = 0x11,
            vk_escape  = 0x1B,
            vk_space   = 0x20,
            vk_left    = 0x25,
            vk_up      = 0x26,
            vk_right   = 0x27,
            vk_down    = 0x28,
        };

        /** \brief Retrieve 'down' key status.
         *
         * \param vk Virtual key code.
         * \return bool The key status.
         *
         */
        static bool keyDown( int vk );
    };
}

/** \brief Apply modifier to output stream.
 *
 * \param out std::ostream&
 * \param mod const con::modifier&
 * \return std::ostream&
 *
 */
DECLDLL std::ostream& operator<<( std::ostream &out, const con::modifier &mod );

#endif // _CONSOLE_H_
