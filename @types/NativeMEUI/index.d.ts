declare module "NativeMEUI" {
    export interface MeuiMouseDownRawEvent {
        type: "mousedown"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseUpRawEvent {
        type: "mouseup"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseMoveRawEvent {
        type: "mousemove"
        x: number
        y: number
        button: number
    }
    export interface MeuiMouseWheelRawEvent {
        type: "mousewheel"
        deltaX: number
        deltaY: number
        deltaZ: number
        button: number
    }

    export type MeuiRawEvent =
        | MeuiMouseDownRawEvent
        | MeuiMouseUpRawEvent
        | MeuiMouseMoveRawEvent
        | MeuiMouseWheelRawEvent

    export class NativeMEUI {
        constructor(width: number, height: number)
        render(root: Box): void
        flush(): void
        update(): void
        debug(): void
        addFontFace(fontFamily: string, path: string): void
        getConnectNumber(): number
        pending(): number
        nextEvent(): MeuiRawEvent | null
    }
    export class Box {
        constructor(type: string)
        getStyle(state: UI_STATE): Record<string, any>
        setStyle(style: Record<string, any>, state: UI_STATE): void
        addChild(child: Box): void
        insertChild(child: Box, index: number): void
        removeChild(child: Box): void
        setState(state: UI_STATE): void
        getState(): UI_STATE
        hit(x: number, y: number): boolean
        get scrollLeft(): number
        set scrollLeft(value: number)
        get scrollTop(): number
        set scrollTop(value: number)
        get scrollWidth(): number
        set scrollWidth(value: number)
        get scrollHeight(): number
        set scrollHeight(value: number)
        get clientWidth(): number
        set clientWidth(value: number)
        get clientHeight(): number
        set clientHeight(value: number)
    }
    export const BOX_STATE: {
        DEFAULT: 0
        FOCUS: 1
        ACTIVE: 2
        HOVER: 3
        SELECT: 4
        DISABLE: 5
        CHECK: 6
    }

    export type UI_STATE = typeof BOX_STATE[keyof typeof BOX_STATE]

    export class Path2D {
        constructor(path?: Path2D | string)

        arc(
            x: number,
            y: number,
            radius: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        arcTo(
            x1: number,
            y1: number,
            x2: number,
            y2: number,
            radius: number
        ): void
        bezierCurveTo(
            cp1x: number,
            cp1y: number,
            cp2x: number,
            cp2y: number,
            x: number,
            y: number
        ): void
        closePath(): void
        ellipse(
            x: number,
            y: number,
            radiusX: number,
            radiusY: number,
            rotation: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        lineTo(x: number, y: number): void
        moveTo(x: number, y: number): void
        quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void
        rect(x: number, y: number, w: number, h: number): void
        // addPath(path: Path2D, transform?: DOMMatrix2DInit): void
    }

    export const Canvas: {
        getImage(
            sx: number,
            sy: number,
            sw: number,
            sh: number,
            settings?: ImageDataSettings
        ): ArrayBuffer
        putImage(
            buffer: ArrayBuffer,
            dx: number,
            dy: number,
            dirtyX: number,
            dirtyY: number,
            dirtyWidth: number,
            dirtyHeight: number,
            width: number,
            height: number,
            format?: number
        ): void
        getWidth(): number
        setWidth(width: number): void
        getHeight(): number
        setHeight(height: number): void

        beginPath(): void
        closePath(): void

        arc(
            x: number,
            y: number,
            radius: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        arcTo(
            x1: number,
            y1: number,
            x2: number,
            y2: number,
            radius: number
        ): void
        bezierCurveTo(
            cp1x: number,
            cp1y: number,
            cp2x: number,
            cp2y: number,
            x: number,
            y: number
        ): void
        ellipse(
            x: number,
            y: number,
            radiusX: number,
            radiusY: number,
            rotation: number,
            startAngle: number,
            endAngle: number,
            counterclockwise?: boolean
        ): void
        lineTo(x: number, y: number): void
        moveTo(x: number, y: number): void

        quadraticCurveTo(cpx: number, cpy: number, x: number, y: number): void
        rect(x: number, y: number, w: number, h: number): void

        stroke(): void
        stroke(path: Path2D): void

        fill(fillRule?: "evenodd" | "nonzero"): void
        fill(path: Path2D, fillRule?: "evenodd" | "nonzero"): void
        clip(fillRule?: "evenodd" | "nonzero"): void
        clip(path: Path2D, fillRule?: "evenodd" | "nonzero"): void

        clearRect(x: number, y: number, w: number, h: number): void
        strokeRect(x: number, y: number, w: number, h: number): void
        fillRect(x: number, y: number, w: number, h: number): void

        setStrokeStyle(r: number, g: number, b: number, a: number): void
        setFillStyle(r: number, g: number, b: number, a: number): void

        setLineCap(lineCap: "butt" | "round" | "square"): void
        setLineJoin(lineJoin: "bevel" | "miter" | "round"): void
        setMiterLimit(miterLimit: number): void
        setLineWidth(lineWidth: number): void
        setLineDash(lineDashOffset: number, segments: number[]): void

        getTransform(): [number, number, number, number, number, number]
        resetTransform(): void
        rotate(angle: number): void
        scale(x: number, y: number): void
        setTransform(
            a: number,
            b: number,
            c: number,
            d: number,
            e: number,
            f: number
        ): void
        setTransform(
            transform?: [number, number, number, number, number, number]
        ): void
        setTransform(a?: any, b?: any, c?: any, d?: any, e?: any, f?: any): void
        transform(
            a: number,
            b: number,
            c: number,
            d: number,
            e: number,
            f: number
        ): void
        translate(x: number, y: number): void
    }
}
