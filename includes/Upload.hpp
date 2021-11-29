#ifndef UPLOAD_HPP
# define UPLOAD_HPP

# include "Headers.hpp"
# include "Post.hpp"

class Upload: public Post
{

	public:
		Upload();
		Upload( Upload const & src );
		Upload( Post const & src );
		~Upload();

		virtual bool	execute(ASocket ** ptr);
		virtual bool	entry(ASocket ** ptr);

	private:
		std::fstream	m_stream;
		size_t			m_stream_size;

		void			_created(void);

};

#endif /* ********************************************************** UPLOAD_H */