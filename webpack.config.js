const UglifyJsPlugin = require("uglifyjs-webpack-plugin");
module.exports = {
    entry: {
        index: "./chip8/index.js",
    },
    devServer: {
        port: 9000
    },
    output: {
        path: __dirname + "/dist/",
        filename: "[name].js"
    },
    node: {
        fs: 'empty'
      },
    module: {
        rules: [
            {
                test: /\.js$/,
                exclude: /node_modules/,
                use: {
                    loader: "babel-loader",
                    options: {
                        presets: ["@babel/preset-env"],
                        plugins: ["@babel/plugin-syntax-dynamic-import", "@babel/transform-runtime"]
                    }
                }
            }
        ]
    },
    optimization: {
        runtimeChunk: false,
        minimizer: [
            new UglifyJsPlugin({
                cache: true,
                parallel: true,
                sourceMap: true
            })
        ]
    }
};
