#ifndef UNCHUNKER_HPP
# define UNCHUNKER_HPP

# include "Headers.hpp"

class Unchunker
{

	public:

		Unchunker();
		Unchunker(Unchunker const & src);
		Unchunker(size_t const & max_size);
		~Unchunker();

		bool	operator()(std::string const & buffer);

	private:
		int				m_chunk_size;
		size_t const	m_max_size;

};

#endif /* ******************************************************* UNCHUNKER_H */