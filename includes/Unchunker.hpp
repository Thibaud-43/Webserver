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

		bool			operator()(std::string & buffer, std::string & body);
		bool const &	getEnd(void) const;
		std::string const	getTotalSize(void) const;

	private:
		std::string		m_chunk_size;
		size_t			m_total_size;
		size_t const	m_max_size;
		bool			m_end;
		size_t			getChunkSize(void);

		bool		checkCharacters(void) const;

};

#endif /* ******************************************************* UNCHUNKER_H */