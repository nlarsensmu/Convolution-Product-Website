
module.exports = {
    dropAlpha: function(floatData) {
        size = floatData.length * 3 / 4; // we are dropping one channel

        var output = new Float32Array(size);
        let idx = 0;
        for (let i = 0; i < floatData.length; i++) {
            if(i % 4 != 3) {
                output[idx] = floatData[i];
                idx++;
            }
        }
        return output;
    },

    duplicte3Channels: function(floatData) {
        size = floatData.length * 3; // we are adding 2 channels

        var output = new Float32Array(size);
        for(let i = 0; i < floatData.length; i++) {
            output[i*3 + 0] = floatData[i]
            output[i*3 + 1] = floatData[i]
            output[i*3 + 2] = floatData[i]
        }
        return output;
    } 
};