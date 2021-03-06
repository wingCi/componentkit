/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#import <Foundation/Foundation.h>
#import <ComponentKit/CKBuildComponent.h>
#import <ComponentKit/CKComponentScopeTypes.h>

@protocol CKTreeNodeProtocol;

@class CKComponent;
@class CKComponentScopeRoot;

/**
 This protocol is being used by the infrastructure to collect data of the components' life cycles.

 Will be used only when systrace is enabled.
 */
@protocol CKSystraceListener <NSObject>
/**
 Called before/after building a scoped component.

 Will be called only when systrace is enabled.
 */
- (void)willBuildComponent:(Class)componentClass;
- (void)didBuildComponent:(Class)componentClass;

/**
 Called before/after mounting a component.

 Will be called only when systrace is enabled.
 */
- (void)willMountComponent:(CKComponent *)component;
- (void)didMountComponent:(CKComponent *)component;

/**
 Called before/after layout a component.

 Will be called only when systrace is enabled.
 */
- (void)willLayoutComponent:(CKComponent *)component;
- (void)didLayoutComponent:(CKComponent *)component;

@end

/**
 This protocol is being used by the infrastructure to collect data about the component tree life cycle.
 */
@protocol CKAnalyticsListener <NSObject>

/**
 Called before the component tree creation

 @param scopRoot Scope root for component tree. Use that to identify tree between will/didBuild.
 @param buildTrigger The build trigger (new tree, state update, props updates) for this component tree creation.
 @param stateUpdates The state updates map for the component tree creation.
 */
- (void)willBuildComponentTreeWithScopeRoot:(CKComponentScopeRoot *)scopeRoot
                               buildTrigger:(BuildTrigger)buildTrigger
                               stateUpdates:(const CKComponentStateUpdateMap &)stateUpdates;

/**
 Called after the component tree creation

 @param scopRoot Scope root for component tree. Use that to identify tree between will/didBuild
 @param component Root component for created tree
 */
- (void)didBuildComponentTreeWithScopeRoot:(CKComponentScopeRoot *)scopeRoot component:(CKComponent *)component;

/**
 Called before/after mounting component tree

 @param component Root component for mounted tree
 */

- (void)willMountComponentTreeWithRootComponent:(CKComponent *)component;
- (void)didMountComponentTreeWithRootComponent:(CKComponent *)component;

/**
 Called before/after component tree layout

 @param component Root component for laid out tree

 @discussion Please not that this callback can be called on the same component from different threads in undefined order, for instance:
             ThreadA, willLayout Component1
             ThreadB, willLayout Component1
             ThreadA, didLayout Component1
             ThreadB, didLayout Component1
             To identify matching will/didLayout events between callbacks, please use Thread id and Component id
 */

- (void)willLayoutComponentTreeWithRootComponent:(CKComponent *)component;
- (void)didLayoutComponentTreeWithRootComponent:(CKComponent *)component;

/** Render Components **/

/**
 Called after a component tree's node has been reused

 @param node The tree node that has been reused.
 @param scopeRoot Scope root for component tree.
 @param previousScopeRoot The previous scope root of the component tree
 @warning A node is only reused if conforming to the render protocol.
 */
- (void)didReuseNode:(id<CKTreeNodeProtocol>)node
         inScopeRoot:(CKComponentScopeRoot *)scopeRoot
fromPreviousScopeRoot:(CKComponentScopeRoot *)previousScopeRoot;

/**
 Provides a systrace listener. Can be nil if systrace is not enabled.
 */
- (id<CKSystraceListener>)systraceListener;

@end
