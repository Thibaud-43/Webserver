# Webserv

### Description
This project is here to make us write our HTTP server.

***
### Conf. File
- Configuration File can not be empty;
- Server Scope can be empty. Here is it's default values below;

| directive  | value |
| ---------- | ------ |
| IP         | 0.0.0.0 |
| Port       | 80 |
| ServerName | localhost |
| BodySizeMax| 1000000 |
| Root       | /var/www/ |
| Methods    | GET POST DELETE | 
| Autoindex  | on |
| Upload     | off |
| Indexes    | {EMPTY} |
| Redirect   | {EMPTY} |
| ErrorPages | {EMPTY} |
| CGI        | {EMPTY} |

- Location Scope can be empty, but URI must be specified. Here is it's default values below;

| directive  | value |
| ---------- | ------ |
| ServerName | {EMPTY} |
| BodySizeMax| {EMPTY} |
| Root       | {EMPTY} |
| Methods    | {EMPTY} |
| Autoindex  | -2 |
| Upload     | -2 |
| Indexes    | {EMPTY} |
| Redirect   | {EMPTY} |
| ErrorPages | {EMPTY} |
| CGI        | {EMPTY} |

- Here's the 12 directives and their formats;

| directive               | Formats |
| ----------              | ------ |
| 1.listen                | Must contain at least IP or Port number, or both (IP must be separated from port with ":") |
| 1a. (IP)                | 0.0.0.0 < X.X.X.X < 255.255.255.255 |
| 1b. (Port)              | 0 < X < 65535 |
| 2. server_name          | Cannot contain "{", "}" or ";" and max names number is 32 |
| 3. client_max_body_size | In bytes and must be inferior at 2^31 bytes |
| 4. cgi                  | first $ARG must be extension and second $ARG must be binary path |
| 5. error_page           | first $ARG must be error code between 301 and 309 (305 excluded) and second $ARG must the file path to redirect |
| 6. methods              | GET, POST and/or DELETE are the only methods accepted |
| 7. index                | Cannot contain "{", "}" or ";" and max indexes number is 32 |
| 8. root                 | Cannot contain "{", "}" or ";" |
| 9. redirect             | first $ARG must be redirect code and second $ARG must the file path to redirect |
| 10. autoindex           | bool ON or OFF |
| 11. upload              | bool ON or OFF |
| 12. location            | first $ARG must be URI and every directive can be called except for listen and server_name. |

- Location blocks will ihnerits from Server block.
- A Location directive cannot be called inside a Location scope;
- A Server directive cannot be called inside a Location scope or a Server scope;
- Several servers can be create inside a cluster;
- Several locations can be create inside a server;
- If a directive meant to be call once, is called several times in the same scope, the last occurence is saved;