import { extend } from '../util/lang';
import generateJSBridgeTrigger from '../inner/triggerjsbridge';

/**
 * 如果api没有runcode，应该有一个默认的实现
 */
export default function callinnerMixin(hybrid) {
    const hybridJs = hybrid;
    const os = hybridJs.os;
    const JSBridge = hybridJs.JSBridge;
    const callJsBridge = generateJSBridgeTrigger(JSBridge);

    /**
     * 专门供API内部调用的，this指针被指向了proxy对象，方便处理
     * @param {Object} options 配置参数
     * @param {Function} resolve promise的成功回调
     * @param {Function} reject promise的失败回调
     */
    function callInner(options, resolve, reject) {
        const data = extend({}, options);

        // 纯数据不需要回调
        data.success = undefined;
        data.fail = undefined;
        data.dataFilter = undefined;

        if (os.syber) {
            // 默认syber环境才触发jsbridge
            callJsBridge(
                {
                    handlerName: this.api.namespace,
                    data,
                    proto: this.api.moduleName,
                    success: options.success,
                    fail: options.fail,
                    dataFilter: options.dataFilter,
                    isLongCb: this.api.isLongCb,
                    isEvent: this.api.isEvent,
                },
                resolve,
                reject,
            );
        }
    }

    hybridJs.callInner = callInner;
}
