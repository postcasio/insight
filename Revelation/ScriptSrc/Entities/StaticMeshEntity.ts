// todo: move decorator to engine
Entity.Register = (info: EntityRegisterInfo) => {
    return (entity: typeof Entity) => {
        return Entity.RegisterInternal(info, entity);
    }
}

export default @Entity.Register({
    Name: "Static Mesh",
    Description: "Static mesh entity",
}) class StaticMeshEntity extends Entity {
    constructor(scene: Scene, name: string) {
        super(scene, name);
        Console.Log("StaticMeshEntity constructor JS");
        this.AddComponent(StaticMeshComponent);
        Console.Log("StaticMeshEntity constructor JS end");
    }
}