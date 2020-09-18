const path = require('path');
const webpack = require('webpack');

const WebPackIgnorePlugin = {
    checkResource: function(resource) {
        const lazyImports = [
            '@nestjs/microservices',
            '@nestjs/microservices/microservices-module',
            '@nestjs/websockets',
            '@nestjs/websockets/socket-module',
            '@nestjs/platform-express',
            'cache-manager',
            'class-validator',
            'class-transformer',
            'fastify-static',
        ];

        if (!lazyImports.includes(resource)) return false;

        try {
            require.resolve(resource);
        } catch (err) {
            return true;
        }

        return false;
    },
};

module.exports = {
    mode: 'production',
    target: 'node',
    entry: './src/main.ts',
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: 'ts-loader',
                exclude: /node_modules/,
            },
        ],
    },
    resolve: {
        extensions: ['.tsx', '.ts', '.js'],
    },
    node: {
        __dirname: false,
    },
    plugins: [new webpack.IgnorePlugin(WebPackIgnorePlugin)],
    optimization: {
        minimize: false,
    },
    output: {
        filename: 'main.js',
        path: path.resolve(__dirname, 'dist'),
    },
};
