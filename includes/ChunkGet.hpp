#ifndef CHUNKGET_HPP
# define CHUNKGET_HPP

# include "Headers.hpp"
# include "Get.hpp"

class ChunkGet: public Get
{

	public:

		ChunkGet();
		ChunkGet(ChunkGet const & src);
		ChunkGet(Get const & src);
		~ChunkGet();

		virtual bool	execute(ASocket ** ptr);
		virtual bool	send_rep(ASocket ** ptr);

	private:
		std::ifstream	m_stream;

		void	_add_chunk_size(std::streamsize const & size);
		bool	_add_read(void);

};

#endif /* ******************************************************** CHUNKGET_H */