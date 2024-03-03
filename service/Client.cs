﻿using Microsoft.AspNetCore.Authentication.OAuth.Claims;
using System.Net;
using System.Net.Sockets;
using System.Text;
using Serilog;
using Microsoft.AspNetCore.Http;
using System.Linq.Expressions;

namespace service
{
    public class Client
    {
        private readonly Serilog.ILogger _logger;
        private IPEndPoint _ipEndPoint;
        private TcpClient _tcpClient;
        private NetworkStream _stream;
        private byte[] _buffer;
        private int _bufferSize;

        public Client(ref byte[] buffer, int bufferSize, Serilog.ILogger logger)
        {
            _logger = logger;
            _ipEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8888);
            _tcpClient = new TcpClient();
            _tcpClient.Connect(_ipEndPoint);
            _stream = _tcpClient.GetStream();
            _buffer = buffer;
            _bufferSize = bufferSize;
        }

        public void SendMessageToServer()
        {
            try
            {
                _stream.Write(_buffer, 0, _bufferSize);
            }
            catch(Exception ex)
            {
                _logger.Error("{0}", ex.Message);
            }
        }

        public byte[]? GetResponseFromServer()
        {
            byte[] buffer = new byte[1024];

            try
            {
                using (MemoryStream memoryStream = new MemoryStream())
                {
                    int bytesRead = _stream.Read(buffer, 0, 1024);

                    memoryStream.Write(buffer, 0, bytesRead);

                    return memoryStream.ToArray();
                }
            }
            catch(Exception ex)
            {
                _logger.Error("{0}", ex.Message);
                return null;
            }
        }
    }
}