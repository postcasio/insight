declare class Scene {

}

declare interface Component<T> {
    Id: string;
    Name: string;

    new(): T;
}


declare class Vector3 {
    constructor(x: number, y: number, z: number);
}

declare class Vector4 {
    constructor(x: number, y: number, z: number, w: number);
}

declare class Quaternion {
    constructor(x: number, y: number, z: number, w: number);
}

declare class TransformComponent {
    static Id: string;
    static Name: string;

    Position: Vector3;
    Quaternion: Quaternion;
    Rotation: Vector3;
    Scale: Vector3;
}

declare class StaticMeshComponent {
    static Id: string;
    static Name: string;
}

declare interface EntityRegisterInfo {
    Name: string;
    Description: string;
}

declare class Entity {
    constructor(scene: Scene, name: string);

    AddComponent<T extends Component<unknown>>(component: T): InstanceType<T>;
    GetComponent<T extends Component<unknown>>(component: T): InstanceType<T>;

    static RegisterInternal<T extends typeof Entity>(info: EntityRegisterInfo, entity: T): T;
    static Register<T extends typeof Entity>(info: EntityRegisterInfo): Entity.RegisterInternal<T>;
}

declare function RegisterEntity<T extends typeof Entity>(entity: T): T;

declare class Console {
    static Log(message: string): void;
}