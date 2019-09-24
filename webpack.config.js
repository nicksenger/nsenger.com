const webpack = require('webpack');
const path = require('path');

module.exports = {
    entry: {
        bundle: ['./src/client/Index.bs.js', './src/client/styles/index.scss'],
        sw: './src/client/sw.bs.js'
    },
    output: {
        path: path.resolve(__dirname, 'src/client/dist'),
        filename: '[name].js',
    },

    devtool: 'source-map',

    resolve: {
        extensions: ['.js', '.scss']
    },

    module: {
        rules: [
            { test: /\.js$/ },
            {
                test: /\.scss$/,
                use: [
                    {
                        loader: 'file-loader',
                        options: {
                            name: 'bundle.css',
                        }
                    },
                    { loader: 'extract-loader' },
                    { loader: 'css-loader' },
                    { loader: 'postcss-loader' },
                    { loader: 'sass-loader' }
                ]
            }
        ]
    }
};